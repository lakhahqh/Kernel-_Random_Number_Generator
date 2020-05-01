- Nếu đang có Kernel sẵn --> Gỡ Driver ra bằng lệnh make out
- Có 2 cách để thực hiện chạy: 
Cách 1: CẤP QUYỀN ADMIN VÀO FILE VCHAR_DEV:
1. make all
2. make in
3. make test
4. make admin
5. ./u_test

Cách 2: Dễ dùng hơn
1. make all
2. make in
3. make test
4. make status

*Các lệnh được rút ngắn cho dễ như là:
make in = sudo insmod character_driver.ko -> lắp driver vào kernel
make out = sudo rmmod character_driver.ko -> tháo driver khỏi kernel
make test = cc -o u_test u_test.c -> biên dịch file menu người dùng
make admin = sudo chmod 666 /dev/vchar_dev : cấp quyền truy cập
make status = sudo ./u_test : Mở file menu