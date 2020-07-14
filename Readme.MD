HỆ ĐIỀU HÀNH
ĐÔ ÁN 1. TÌM HIỂU VÀ LẬP TRÌNH LINUX KERNEL MODULE

Hình thức: đồ án 3 sinh viên
Deadline: 23h00 ngày 29-4-2020
Nộp bài qua  moodle môn học, đặt tên file nộp là MSSV1_MSSV2.zip/rar

Nội dung file nộp gồm:  báo cáo những gì mình hiểu (trình bày trên MSWORD), readme của module đã code, và mã nguồn. 

Nội dung đồ án:
Mục tiêu hiểu về Linux kernel module và hệ thống quản lý file và device trong linux, giao tiếp giữa tiến trình ở user space và code kernel space 
+ Viết một module dùng để tạo ra số ngẫu nhiên. 
+ Module này sẽ tạo một character device để cho phép các tiến trình ở userspace có thể open và read các số ngẫu nhiên.

------------------------------------------------------------------------- Hướng dẫn chạy :D 
Name: Nguyễn Tất Hưng - ID: 18127104

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
