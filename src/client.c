#include "../inc/common.h"

int main() {
    int server_fd, client_fd;
    Message req_msg, res_msg;
    char client_fifo_name[256];

    // Lấy PID của chính Client này
    pid_t my_pid = getpid();

    // Bước 2: Tạo FIFO dữ liệu riêng biệt (Dedicated FIFO) cho Client
    sprintf(client_fifo_name, CLIENT_FIFO_TEMPLATE, my_pid);
    if (mkfifo(client_fifo_name, 0666) == -1) {
        perror("Client: Tạo kênh riêng thất bại");
        exit(1);
    }
    printf("CLIENT [%d]: Đã tạo kênh nhận dữ liệu riêng tư tại %s\n", my_pid, client_fifo_name);

    // Bước 3: Đóng gói bản tin Request
    req_msg.client_pid = my_pid;
    sprintf(req_msg.data, "Xin chào, tôi cần dịch vụ!");
    req_msg.data_length = strlen(req_msg.data) + 1;

    // Mở FIFO gốc của Server để ném yêu cầu vào
    server_fd = open(SERVER_FIFO, O_WRONLY);
    if (server_fd == -1) {
        perror("Client: Không tìm thấy Server");
        unlink(client_fifo_name); // Dọn dẹp FIFO rác
        exit(1);
    }

    printf("CLIENT [%d]: Đang gửi yêu cầu tới Server...\n", my_pid);
    write(server_fd, &req_msg, sizeof(Message));
    close(server_fd); // Đóng đầu ghi khi đã gửi xong

    // Mở FIFO riêng tư ở chế độ đọc để chờ Server trả lời (bị block tại đây)
    client_fd = open(client_fifo_name, O_RDONLY);
    if (client_fd != -1) {
        if (read(client_fd, &res_msg, sizeof(Message)) > 0) {
            printf("CLIENT [%d]: Server [PID: %d] phản hồi: %s\n", 
                   my_pid, res_msg.client_pid, res_msg.data);
        }
        close(client_fd);
    }

    // Kết thúc: Tự động hủy bỏ FIFO riêng tư của mình để dọn dẹp hệ thống
    unlink(client_fifo_name);
    printf("CLIENT [%d]: Đã dọn dẹp kênh riêng và thoát.\n", my_pid);

    return 0;
}

