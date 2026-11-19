#!/usr/bin/env python3
# ============================================================================
# File: serve.py
# C/C++ Handbook: local dev server + static site builder
# 100% Python Standard Library (Zero external dependencies)
#
#   python3 serve.py                 # local preview at http://127.0.0.1:8000
#   python3 serve.py --build _site   # write a static site (GitHub Pages)
#
# The frontend only ever reads static JSON under data/:
#   data/curriculum.json, data/modules/<id>.json, data/search-index.json
# The dev server generates those on the fly from the READMEs and source files,
# and --build writes the very same files to disk.
# ============================================================================

import argparse
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
import json
from pathlib import Path
import shutil
from urllib.parse import unquote, urlparse

ROOT_DIR = Path(__file__).resolve().parent
WEB_DIR = ROOT_DIR / "web"

STAGES_CONFIG = [
    {
        "stageId": "overview",
        "stageCode": "00",
        "stageTitle": "Tổng quan Lộ trình",
        "folders": ["ROOT"],
    },
    {
        "stageId": "stage-1",
        "stageCode": "01",
        "stageTitle": "Nền tảng C/C++ & Tư duy Lập trình",
        "folders": [
            "01_Compilation_Process",
            "02_Variables_DataTypes_Operators",
            "03_Control_Flow",
            "04_Functions_and_Scope",
            "05_Arrays_1D_2D",
            "MiniProject1_Grade",
        ],
    },
    {
        "stageId": "stage-2",
        "stageCode": "02",
        "stageTitle": "Bộ nhớ, Con trỏ & Thao tác cấp thấp",
        "folders": [
            "06_Memory_Layout",
            "07_Pointers_and_References",
            "08_Dynamic_Memory_Allocation",
            "09_Strings_C_and_CPP",
            "10_Struct_Union_Enum",
            "11_Recursion_and_Backtracking",
            "12_File_IO_C_and_CPP",
            "MiniProject2_StudentManager",
        ],
    },
    {
        "stageId": "stage-3",
        "stageCode": "03",
        "stageTitle": "Lập trình Hướng đối tượng (OOP)",
        "folders": [
            "13_OOP_Class_and_Object",
            "14_OOP_Four_Pillars",
            "15_OOP_Resource_Management",
            "MiniProject3_Payroll",
        ],
    },
    {
        "stageId": "stage-4",
        "stageCode": "04",
        "stageTitle": "Cấu trúc dữ liệu & Giải thuật (DSA)",
        "folders": [
            "16_DSA_Algorithm_Complexity",
            "17_DSA_Sorting_and_Searching",
            "18_DSA_Linear_Data_Structures",
            "19_DSA_Trees",
            "20_DSA_Hash_Table",
            "21_DSA_Graph_Algorithms",
            "22_DSA_Algorithm_Design_Paradigms",
            "MiniProject4_PathFinder",
        ],
    },
    {
        "stageId": "stage-5",
        "stageCode": "05",
        "stageTitle": "Lập trình Tổng quát & Thư viện STL",
        "folders": [
            "23_Templates_and_Generic_Programming",
            "24_STL_Sequence_Containers",
            "25_STL_Associative_Unordered_Adapters",
            "26_STL_Algorithms_and_Numeric",
            "27_STL_Functors",
        ],
    },
    {
        "stageId": "stage-6",
        "stageCode": "06",
        "stageTitle": "C++ Hiện đại (C++11/14/17/20)",
        "folders": [
            "28_Modern_CPP_Syntax_and_Types",
            "29_Modern_CPP_Lambda_Expressions",
            "30_Modern_CPP_RAII_and_Smart_Pointers",
            "31_Modern_CPP_Move_Semantics",
            "32_Modern_CPP20_Features",
        ],
    },
    {
        "stageId": "stage-7",
        "stageCode": "07",
        "stageTitle": "Đa luồng, Design Patterns & SOLID",
        "folders": [
            "33_Multithreading_and_Concurrency",
            "34_Design_Patterns_Creational",
            "35_Design_Patterns_Structural",
            "36_Design_Patterns_Behavioral",
            "37_SOLID_Principles",
        ],
    },
]


def extract_markdown_title(readme_path: Path, fallback: str) -> str:
    if not readme_path.exists():
        return fallback
    try:
        for line in readme_path.read_text(encoding="utf-8").splitlines():
            line = line.strip()
            if line.startswith("# "):
                return line[2:].strip()
    except Exception:
        pass
    return fallback


def build_curriculum_data():
    stages_out = []
    total_modules = 0
    total_code_files = 0

    for st in STAGES_CONFIG:
        modules_list = []
        for folder in st["folders"]:
            if folder == "ROOT":
                readme_path = ROOT_DIR / "README.md"
                modules_list.append({
                    "id": "ROOT",
                    "folder": "",
                    "code": "00",
                    "isMiniProject": False,
                    "title": extract_markdown_title(readme_path, "Lộ trình Học C/C++ Toàn diện"),
                    "fileCount": 0,
                    "files": [],
                })
                continue

            mod_dir = ROOT_DIR / folder
            if not mod_dir.is_dir():
                continue

            readme_path = mod_dir / "README.md"
            code_files = sorted(
                [p.name for p in mod_dir.iterdir() if p.is_file() and p.suffix in (".c", ".cpp", ".h", ".hpp")]
            )
            total_modules += 1
            total_code_files += len(code_files)

            is_mp = folder.startswith("MiniProject")
            code_tag = "MP" + folder.split("_")[0].replace("MiniProject", "") if is_mp else folder.split("_")[0]

            modules_list.append({
                "id": folder,
                "folder": folder,
                "code": code_tag,
                "isMiniProject": is_mp,
                "title": extract_markdown_title(readme_path, folder),
                "fileCount": len(code_files),
                "files": code_files,
            })

        stages_out.append({
            "stageId": st["stageId"],
            "stageCode": st["stageCode"],
            "stageTitle": st["stageTitle"],
            "modules": modules_list,
        })

    return {
        "totalModules": total_modules,
        "totalCodeFiles": total_code_files,
        "stages": stages_out,
    }


def get_module_detail(module_id: str):
    if module_id == "ROOT":
        readme_path = ROOT_DIR / "README.md"
        readme_text = readme_path.read_text(encoding="utf-8") if readme_path.exists() else ""
        return {
            "id": "ROOT",
            "title": extract_markdown_title(readme_path, "Lộ trình Học C/C++ Toàn diện"),
            "readme": readme_text,
            "files": [],
        }

    if "/" in module_id or "\\" in module_id or ".." in module_id:
        return None
    mod_dir = ROOT_DIR / module_id
    if not mod_dir.is_dir():
        return None

    readme_path = mod_dir / "README.md"
    readme_text = readme_path.read_text(encoding="utf-8") if readme_path.exists() else ""

    files_data = []
    for p in sorted(mod_dir.iterdir()):
        if p.is_file() and p.suffix in (".c", ".cpp", ".h", ".hpp"):
            content = p.read_text(encoding="utf-8", errors="replace")
            files_data.append({
                "name": p.name,
                "lang": "c" if p.suffix == ".c" else "cpp",
                "lines": len(content.splitlines()),
                "content": content,
            })

    return {
        "id": module_id,
        "title": extract_markdown_title(readme_path, module_id),
        "readme": readme_text,
        "files": files_data,
    }


def build_search_index():
    """Everything the in-browser search needs, in curriculum order."""
    index = []
    for st in build_curriculum_data()["stages"]:
        for mod in st["modules"]:
            detail = get_module_detail(mod["id"])
            if detail is None:
                continue
            index.append({
                "moduleId": mod["id"],
                "moduleTitle": mod["title"],
                "readme": detail["readme"],
                "files": [{"name": f["name"], "content": f["content"]} for f in detail["files"]],
            })
    return index


def static_data(path: str):
    """Map a data/ path to its JSON payload, or None if it doesn't exist."""
    if path == "data/curriculum.json":
        return build_curriculum_data()
    if path == "data/search-index.json":
        return build_search_index()
    if path.startswith("data/modules/") and path.endswith(".json"):
        return get_module_detail(path[len("data/modules/"):-len(".json")])
    return None


def dump_json(data) -> bytes:
    return json.dumps(data, ensure_ascii=False, separators=(",", ":")).encode("utf-8")


def build_site(out_dir: Path):
    """Write web/ plus all data/*.json into out_dir, ready for any static host."""
    # out_dir gets wiped, so refuse anything that could be the repo or a source dir
    if out_dir in (ROOT_DIR, WEB_DIR) or out_dir in ROOT_DIR.parents or WEB_DIR in out_dir.parents \
            or any((ROOT_DIR / f) == out_dir for st in STAGES_CONFIG for f in st["folders"]):
        raise SystemExit(f"Refusing to build into {out_dir}: it is (or contains) project sources.")
    if out_dir.exists():
        if any(out_dir.iterdir()) and not (out_dir / ".nojekyll").exists():
            raise SystemExit(f"Refusing to overwrite {out_dir}: not empty and not a previous build.")
        shutil.rmtree(out_dir)
    shutil.copytree(WEB_DIR, out_dir)
    (out_dir / ".nojekyll").write_text("")  # serve files as-is on GitHub Pages

    curriculum = build_curriculum_data()
    paths = ["data/curriculum.json", "data/search-index.json"]
    paths += [
        f"data/modules/{mod['id']}.json"
        for st in curriculum["stages"]
        for mod in st["modules"]
    ]
    for rel in paths:
        target = out_dir / rel
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(dump_json(static_data(rel)))

    total = sum(f.stat().st_size for f in out_dir.rglob("*") if f.is_file())
    print(f"Built {len(paths)} data files into {out_dir} ({total / 1024:.0f} KiB total)")


class PortalRequestHandler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=str(WEB_DIR), **kwargs)

    def log_message(self, fmt, *args):
        pass

    def do_GET(self):
        path = unquote(urlparse(self.path).path).lstrip("/")
        if path.startswith("data/"):
            data = static_data(path)
            if data is None:
                self.send_error(404)
                return
            raw = dump_json(data)
            self.send_response(200)
            self.send_header("Content-Type", "application/json; charset=utf-8")
            self.send_header("Content-Length", str(len(raw)))
            self.send_header("Cache-Control", "no-store")
            self.end_headers()
            self.wfile.write(raw)
            return
        return super().do_GET()


def main():
    parser = argparse.ArgumentParser(description="C/C++ Handbook dev server / static site builder")
    parser.add_argument("--port", type=int, default=8000)
    parser.add_argument("--host", type=str, default="127.0.0.1")
    parser.add_argument("--build", metavar="DIR", help="write a static site to DIR and exit")
    args = parser.parse_args()

    if args.build:
        build_site(Path(args.build).resolve())
        return

    server = ThreadingHTTPServer((args.host, args.port), PortalRequestHandler)
    print(f"C/C++ Handbook listening on http://{args.host}:{args.port}")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        server.server_close()


if __name__ == "__main__":
    main()
