#!/usr/bin/env bash
set -e
LIBS=(-lCSvs -lDecodeSdpc -lDecodeHevc -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswresample -lswscale -lx264 -lx265 -ljpeg -lturbojpeg)
g++ src/sdpc2svs.cpp "${LIBS[@]}" -o sdpc2svs -D LINUX  -I include "${LIBS[@]}" \
-L "./lib/" -L "./lib/jpeg" -L "./lib/ffmpeg" \
-Wl,-rpath,"\$ORIGIN/lib/" -Wl,-rpath,"\$ORIGIN/lib/jpeg" -Wl,-rpath,"\$ORIGIN/lib/ffmpeg" -Wl,-disable-new-dtags

