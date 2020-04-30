# `osh` (Simple Shell)



### Thông tin đồ án

-   Đồ án cho môn Hệ Điều Hành (CSC10007-18CLC2), FIT @ HCMUS
-   Nhóm gồm 3 thành viên
    -   `18127231`: Đoàn Đình Toàn, [@t3bol90](https://github.com/t3bol90)
    -   `18127185`: Bùi Vũ Hiếu Phụng, [@alecmatts](https://github.com/alecmatts)
    -   `18127221`: Bùi Văn Thiện, [@84436](https://github.com/84436)



### Công việc cần làm

| Tính năng                               | Toàn | Phụng | Thiện |
| --------------------------------------- | ---- | ----- | ----- |
| **Parse**                               | x    |       | x     |
| **Thực thi lệnh**                       | x    |       |       |
| `&` **(thực thi lệnh dưới nền)**        | x    |       |       |
| `<`, `>`, `<<` **(chuyển hướng I/O)**   | x    |       | x     |
| `|` **(pipe)**                          | x    |       |       |
| **Lịch sử lệnh (có thể thay đổi size)** |     | x     |       |
| `PS1` (shell prompt)                    |      | x     | x     |
| `cd` (chuyển thư mục)                   | x    | x     |       |
| `help`                                  |      |       | x     |
| Makefile                                |      |       | x     |



### Hướng thiết kế của đồ án
##### Ngôn ngữ và môi trường lập trình

Đồ án được lập trình bằng ngôn ngữ `C`,biên dịch bằng `gcc` (sử dụng Makefile) trên môi trường `Ubuntu` (version `18.04` trở lên) với 2 IDE được sử dụng bởi các thành viên trong nhóm là `Visual Studio Code` và `CLion`.



##### Kịch bản của chương trình

1. Khi shell bắt đầu chạy, nó sẽ vào một vòng lặp Read-Exec (đọc lệnh - thực thi) cho tới khi nhận được lệnh `exit`
2. Đối vối Read: shell sẽ thông qua `parse2()` để đọc-hiểu lệnh
   - Mặc định, `parse2()` nhận nhóm {lệnh + tham số} đầu tiên
   - Nếu có sử dụng op `|` (pipe) hoặc chuyển hướng I/O tương ứng, `parse2()` nhận tiếp nhóm {lệnh + tham số} thứ hai và op đã được đưa vào
   - Nếu có sử dụng op `&` (thực thi dưới nền), `parse2()` trả về op đã sử dụng trong trường hợp này
   - Nếu có nhiều hơn 1 op xuất hiện trong command và/hoặc op đặt ở vị trí không thích hợp: `parse2()` đánh dấu lệnh được đưa là "không hợp lệ/không được hỗ trợ" và ngừng parse
3. Đối với Exec: Shell sẽ dựa vào thông tin đã nhận được ở `parse2()` để fork shell hiện tại ra và, tùy thuộc vào việc lệnh đó có phải là `built-in` của shell hay không, sẽ hành động tương ứng.
   - Nếu như lệnh là `built-in`: đưa lệnh vào một trong các hàm `built_in_*` tương ứng.
   - Nếu không phải là `built-in`:
     - Nếu như lệnh được đánh dấu là "không hợp lệ/không được hỗ trợ":  bỏ qua lệnh và báo lỗi
     - Trong các trường hợp còn lại, các nhóm  {lệnh + tham số} sẽ được đưa vào một trong các hàm `child_*` tương ứng với op đã phát hiện được trong lệnh.



##### Mô hình của `parse2()`

`parse2()` có tên như vậy là vì nó là bản cài đặt lại của `parse()` để có thể nhận được các op (`&`, `|`, `<`, `>` và `>>`) mà _không sử dụng `strtok()`, `strchr()` hay các công cụ để tokenize/tìm kí tự trong chuỗi khác trong thư viện xử lí C string có sẵn_.



##### Hướng cài đặt/mô hình của các lệnh

- Thực thi
  - `child()`
- `&` (Thực thi dưới nền)
  - Tham số `wait` trên tiến trình chính`parent()`
- `<`, `>`, `>>` (Chuyển hướng I/O)
  - `child_fromfile()`
  - `child_tofile()`
- `|` (Pipe)
  - `child_pipe()`
- Lịch sử lệnh:
  Lịch sử lệnh được cài đặt dưới dạng một mảng tĩnh nhưng "thay đổi" kích thước được (thông qua `histsize`). Các lệnh được đánh số tăng dần theo thứ tự từ lệnh cũ nhất tới mới nhất.
  - `histsize`
- Các tính năng linh tinh
  - `PS1` (Shell prompt)
    Đây chỉ là một string để hiển thị trước dấu nhắc.
  - `cd` (Thay đổi thư mục làm việc)
    Việc thay đổi thư mục hiện tại được thực hiện bằng `chdir()` trong `<unistd.h>`
  - `help` (Hướng dẫn cho các lệnh có sẵn)
    Các help messages được cài đặt dưới dạng `static` string. Hàm help chỉ thực hiện một phép so sánh cơ bản trên tham số nhận được (lệnh cụ thể để tra cứu) nếu cần, sau đó hiện help message tương ứng nếu lệnh tồn tại, hoặc thông báo lỗi nếu lệnh không tồn tại.



### Nhận xét của nhóm

##### Về lựa chọn C + Makefile

- C là ngôn ngữ được dùng để tạo ra Linux kernel ở thuở sơ khai. Đến ngày nay, bản thân Linux và một số công cụ chạy trên nó được viết bằng C.

- > Nhóm thích sự thử thách bản thân với `pointer`, `malloc`,... những thứ mà `C++` hay những ngôn ngữ khác không có được hoặc khó để trải nghiệm. Đồ án này đơn giản là một trải nghiệm, thế thôi :>

  -- Toàn Đoàn, lead nhóm

- Makefile là một công cụ truyền thống hỗ trợ việc tự động các tác vụ khi compile chương trình trên Linux/*nix. Makefile ngắn gọn (nhưng không dễ đọc :<) và tương đối dễ sử dụng hơn CMake; ta có thể xem nó như một cái script (nhưng với cú pháp không dễ đọc/viết :<)



##### Về lớp lệnh built-in

Các lệnh built-in của shell hiện chỉ mang tính sơ khai và phục vụ một số tính năng phụ (như thay đổi kích thước lịch sử lệnh sử dụng `histsize`, xem hướng dẫn sử dụng `histsize`, v.v.). Lớp lệnh này có thể phát triển thêm nếu có thời gian mở rộng.




### Hướng dẫn sử dụng

Tham khảo `help` có sẵn trong `osh` để xem hướng dẫn sử dụng.

