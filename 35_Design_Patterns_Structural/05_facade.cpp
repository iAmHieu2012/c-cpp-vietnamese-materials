#include <iostream>
#include <string>

// =============================================================================
// FACADE PATTERN (GIAO DIỆN ĐƠN GIẢN HÓA HỆ THỐNG CON PHỨC TẠP)
// =============================================================================
struct AudioDecoder {
    void decode_audio(const std::string &src) const {
        std::cout << "    1. [AudioDecoder]  Tach va giai ma kenh am thanh AAC tu '" << src << "'...\n";
    }
};

struct VideoTranscoder {
    void transcode_h264() const {
        std::cout << "    2. [VideoTranscoder] Chuyen doi khung hinh sang chuan H.264 1080p...\n";
    }
};

struct SubtitleInjector {
    void burn_subtitles() const {
        std::cout << "    3. [SubtitleInjector] Ghep phu de tieng Viet vao khung hinh...\n";
    }
};

struct FileMuxer {
    void mux_mp4(const std::string &out) const {
        std::cout << "    4. [FileMuxer]       Dong goi hoan tat file: " << out << "\n";
    }
};

// Lớp Facade gom toàn bộ quy trình 4 bước của hệ thống con vào đúng 1 hàm đơn giản
class VideoConverterFacade {
private:
    AudioDecoder     audio;
    VideoTranscoder  video;
    SubtitleInjector subs;
    FileMuxer        muxer;

public:
    void convert_video_1click(const std::string &input_raw, const std::string &output_mp4) const {
        std::cout << "  [VideoConverterFacade] Bat dau chuyen doi '" << input_raw << "':\n";
        audio.decode_audio(input_raw);
        video.transcode_h264();
        subs.burn_subtitles();
        muxer.mux_mp4(output_mp4);
    }
};

int main() {
    std::cout << "=== FACADE PATTERN (1-CLICK VIDEO CONVERTER) ===\n";
    VideoConverterFacade converter;
    converter.convert_video_1click("raw_recording.mkv", "final_lecture_cpp17.mp4");

    return 0;
}
