mkdir -p /usr/lib/x86_64-linux-gnu/pkgconfig/archive
mv /usr/lib/x86_64-linux-gnu/pkgconfig/libssl.pc /usr/lib/x86_64-linux-gnu/pkgconfig/archive
mkdir -p /usr/lib/x86_64-linux-gnu/archive
mv \
/usr/lib/x86_64-linux-gnu/libssl.so.1.1 \
/usr/lib/x86_64-linux-gnu/libssl.so.3 \
/usr/lib/x86_64-linux-gnu/libssl.a \
/usr/lib/x86_64-linux-gnu/libssl.so \
/usr/lib/x86_64-linux-gnu/libssl3.so \
/usr/lib/x86_64-linux-gnu/archive
