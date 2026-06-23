#include "../inc/common.h"

int main() {
    int server_fd, client_fd;
    Message req_msg, res_msg;
    char client_fifo_name[256];

    // Tạo FIFO gốc cho Server (nếu chưa có)
    if (mkfifo(SERVER_FIFO, 0666) == -1) {
        perror("Server: mkfifo gốc thất bại (có thể đã tồn tại)");
    }

    printf("SERVER: Đang khởi động và lắng nghe tại %s...\n", SERVER_FIFO);

    while (1) {
        // Mở FIFO gốc ở chế độ chỉ đọc. Sẽ bị block ở đây cho đến khi có Client gửi tới.
        server_fd = open(SERVER_FIFO, O_RDONLY);
        if (server_fd == -1) {
            perror("Server: Lỗi mở FIFO gốc");
            exit(1);
        }

        // Đọc bản tin request từ Client (bóc tách Header và Data)
        if (read(server_fd, &req_msg, sizeof(Message)) > 0) {
            printf("\nSERVER: Nhận được yêu cầu từ Client [PID: %d]\n", req_msg.client_pid);
            printf("SERVER: Nội dung: %s (Kích thước: %d byte)\n", req_msg.data, req_msg.data_length);

            // Xử lý logic (Mô phỏng: Server phản hồi lại bằng cách ghép thêm chuỗi ACK)
            res_msg.client_pid = getpid(); // Gửi PID của Server về
            sprintf(res_msg.data, sizeof(res_msg.data), "ACK từ Server! Đã nhận: %s", req_msg.data);
            res_msg.data_length = strlen(res_msg.data) + 1;

            // Lấy tên FIFO riêng tư của Client dựa vào PID gửi lên
            sprintf(client_fifo_name, CLIENT_FIFO_TEMPLATE, req_msg.client_pid);

            // Mở FIFO riêng tư của Client để ghi (bơm dữ liệu thẳng vào kênh riêng)
            client_fd = open(client_fifo_name, O_WRONLY);
            if (client_fd != -1) {
                write(client_fd, &res_msg, sizeof(Message));
                close(client_fd);
                printf("SERVER: Đã gửi phản hồi vào kênh riêng %s\n", client_fifo_name);
            } else {
                perror("Server: Không thể mở FIFO riêng của Client");
            }
        }
        close(server_fd); // Đóng lại để sẵn sàng cho yêu cầu tiếp theo
    }
    
    unlink(SERVER_FIFO); // Dọn dẹp hệ thống khi Server tắt
    return 0;
}
