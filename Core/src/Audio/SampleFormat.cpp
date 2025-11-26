#include "SampleFormat.hpp"

namespace Pixf::Core::Audio {
    ma_format ToMaFormat(const SampleFormat &format) {
        ma_format maFmt = ma_format_unknown;

        switch (format) {
            case SampleFormat::Default:
                maFmt = ma_format_unknown;
                break;
            case SampleFormat::Float32:
                maFmt = ma_format_f32;
                break;
            case SampleFormat::Int16:
                maFmt = ma_format_s16;
                break;
            case SampleFormat::Int24:
                maFmt = ma_format_s24;
                break;
            case SampleFormat::Int32:
                maFmt = ma_format_s32;
                break;
            case SampleFormat::UInt8:
                maFmt = ma_format_u8;
                break;
        }

        return maFmt;
    }

    SampleFormat FromMaFormat(const ma_format &format) {
        auto outFormat = SampleFormat::Default;

        switch (format) {
            case ma_format_unknown:
                outFormat = SampleFormat::Default;
                break;
            case ma_format_f32:
                outFormat = SampleFormat::Float32;
                break;
            case ma_format_s32:
                outFormat = SampleFormat::Int32;
                break;
            case ma_format_s24:
                outFormat = SampleFormat::Int24;
                break;
            case ma_format_s16:
                outFormat = SampleFormat::Int16;
                break;
            case ma_format_u8:
                outFormat = SampleFormat::UInt8;
                break;
            default:
                outFormat = SampleFormat::Default;
                break;
        }

        return outFormat;
    }
} // namespace Pixf::Core::Audio
