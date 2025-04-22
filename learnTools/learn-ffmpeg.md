- [ffmpeg](https://ffmpeg.org)
# [硬件加速](https://trac.ffmpeg.org/wiki/HWAccelIntro)

这个链接约等于翻译的上述官网链接：https://zhuanlan.zhihu.com/p/645241136
主要针对ffmpeg支持的硬解码做一个总结阐述。

许多平台提供对专用硬件的访问，以执行一系列与视频相关的任务。使用此类硬件可以更快地完成某些操作，例如解码、编码或过滤，或者使用更少的其他资源（尤其是 CPU），但可能会产生不同或较差的结果，或者施加仅使用软件时不存在的额外限制。在类似 PC 的平台上，视频硬件通常集成到 GPU（来自 AMD、Intel 或 NVIDIA）中，而在移动 SoC 类型的平台上，它通常是一个独立的 IP 核

硬件解码器将产生与软件解码器相同的输出，但可能使用更少的功率和 CPU 来这样做。功能支持各不相同——对于具有许多不同配置文件的更复杂的编解码器，硬件解码器很少实现所有这些（例如，硬件解码器往往不会在 H.264 的 8 位深度下实现 YUV 。许多硬件解码器的一个共同特点是能够在适合其他组件使用的硬件表面中生成输出（对于独立显卡，这意味着表面在卡上的内存中而不是在系统内存中）——这通常对播放很有用，因为在呈现输出之前不需要进一步复制，并且在某些情况下，它还可以与支持硬件表面输入的编码器一起使用，以避免在转码情况下进行任何复制。

硬件编码器生成的输出质量通常比好的软件编码器（如 x264）低得多，但通常速度更快并且不使用太多 CPU 资源。（也就是说，它们需要更高的比特率才能以相同的感知质量进行输出，或者在相同的比特率下以较低的感知质量进行输出。）

具有解码和/或编码能力的系统还可以提供对其他相关过滤功能的访问。像缩放和去隔行这样的事情很常见，其他后处理可能取决于系统。在硬件表面可用的情况下，这些过滤器通常会作用于它们而不是系统内存中的正常帧。

## 通用选项
```bash
#查看可用的硬件加速
ffmpeg -hwaccels
```

## Nvidia-NVDEC/CUVID/CUDA
**NVENC and NVDEC are NVIDIA's hardware-accelerated encoding and decoding APIs. They used to be called CUVID. They can be used for encoding and decoding on Windows and Linux. FFmpeg refers to NVENC/NVDEC interconnect as CUDA.**

NVDEC 为 H.264、HEVC、MJPEG、MPEG-1/2/4、VP8/VP9、VC-1 提供解码器。编解码器支持硬件（见GPU兼容性表）。

请注意，FFmpeg 提供 NVDEC 和 CUVID hwaccels。它们的不同之处在于帧在内存中的解码和转发方式。

全套编解码器仅在 Pascal 硬件上可用，它增加了 VP9 和 10 位支持。关于 NVENC 缺少 ffnvcodec 的说明也适用于 NVDEC。

使用 CUDA 的解码示例：
```bash
ffmpeg -hwaccel cuda -i input output
```

使用 CUVID 的解码示例：
```bash
ffmpeg -c:v h264_cuvid -i input output
```

使用 NVDEC 和 NVENC 进行全硬件转码：
```bash
ffmpeg -hwaccel cuda -hwaccel_output_format cuda -i input -c:v h264_nvenc -preset slow output
```

如果 ffmpeg 是在支持 libnpp 的情况下编译的，则它可用于将基于 GPU 的缩放器插入链中：
```bash
ffmpeg -hwaccel_device 0 -hwaccel cuda -i input -vf scale_npp=-1:720 -c:v h264_nvenc -preset slow output.mkv
```

该-hwaccel_device选项可用于指定 ffmpeg 中硬解码要使用的 GPU。

## Mac-videotoolbox
[硬件加速转码mac-ffmpeg](https://www.nxrte.com/jishu/33751.html)

**全链路硬件加速转码示例如下：**

```
./ffmpeg -hwaccel videotoolbox \
  -hwaccel_output_format videotoolbox_vld \
  -i ios-265.mov \
  -c:v hevc_videotoolbox \
  -profile:v main \
  -b:v 3M \
  -vf scale_vt=w=iw/2:h=ih/2:color_matrix=bt709:color_primaries=bt709:color_transfer=bt709 \
  -c:a copy \
  -tag:v hvc1 \
  /tmp/test.mp4
```

- -hwaccel videotoolbox 提供videotoolbox硬件上下文，主要功能是硬件内存buffer分配；它的另一个功能是封装了CPU和GPU之间的数据传输拷贝，但在这个例子中用不到
- -hwaccel_output_format videotoolbox_vld 指定了硬件输出图像格式；
    - 如不指定，一般默认会选中NV12
    - **指定了videotoolbox_vld，FFmpeg不会自动插入CPU filter，图像在GPU内存，如果需要的话，可以用hwdownload filter下载到CPU**
    - **用软件解码器的时候，也可以用硬件filter，只需要先hwupload到GPU**
- FFmpeg videotoolbox视频解码不是单独的一个解码器，是在h264、h265解码器上的加速。有几个硬件视频解码加速都是这么实现的，一个公共的管理调度 + 不同的硬件加速。最新的是av1解码，FFmpeg内置的av1解码器，没有软件实现，全靠硬件加速完成解码功能。指定了-hwaccel videotoolbox之后，FFmpeg自动启用videotoolbox解码
- -c:v hevc_videotoolbox指定编码器
- -vf scale_vt=w=iw/2:h=ih/2:color_matrix=bt709:color_primaries=bt709:color_transfer=bt709
    - scale_vt是用VTPixelTransferSession实现的filter
    - w=iw/2:h=ih/2 宽高缩小一半
    - color_matrix=bt709:color_primaries=bt709:color_transfer=bt709 进行色彩空间转换
- -tag:v hvc1与mp4封装有关，Apple平台要求hvc1，不指定的话默认是hev1，Apple系统播放器不支持

示例中，输入文件是4K HDR视频，

Input: hevc (Main 10) (hvc1 / 0x31637668), yuv420p10le(tv, bt2020nc/bt2020/arib-std-b67), 3840×2160

缩放加色彩空间转换之后，视频信息如下：

Output: hevc (Main) (hvc1 / 0x31637668), yuv420p(tv, bt709, progressive), 1920×1080

整个流程性能接近最优。编码器本身还有一些参数配置可以优化，以后再分析。

## Intel-VAAPI
https://cloud.tencent.com/developer/article/1871152

全称：Video Acceleration API，视频加速 API (VAAPI) 是一种非专有且免版税的开源软件库 (“libva”) 和 API 规范，最初由英特尔开发，但可与其他设备结合使用。

它可用于访问 Intel GPU 中的 Quick Sync 硬件和 AMD GPU 中的 UVD/VCE 硬件。请参阅VAAPI。
