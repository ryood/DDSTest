rem DDSTest-12bitTable.exe > DDSTest-12bitTable.raw
rem D:\home\app\sox12177\sox.exe -s -w -r 44100 -c 1 "DDSTest-12bitTable.raw" "DDSTest-12bitTable.wav"

Debug\DDSTest-LFO.exe > DDSTest-LFO.raw
..\RawRateConvert\Debug\RawRateCOnvert.exe < DDSTest-LFO.raw > DDSTest-LFO-44100.raw
D:\home\app\sox12177\sox.exe -s -w -r 44100 -c 1 "DDSTest-LFO-44100.raw" "DDSTestLFO.wav"
