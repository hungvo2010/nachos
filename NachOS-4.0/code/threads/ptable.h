#ifndef PTABLE_H
#define PTABLE_H
#include "bitmap.h"
#include "pcb.h"
#include "synch.h"
#define MAX_PROCESS 10
class Semaphore;
class PCB;

class PTable {
   private:
    Bitmap* bm;

    PCB* pcb[MAX_PROCESS];
    int psize;
    Semaphore* bmsem;  // dùng để ngăn chặn trường hợp nạp 2 tiến trình cùng lúc.
   public:
    AddrSpace* addrspace[MAX_PROCESS];
    PTable(int size);
    // Khởi tạo size đối tượng pcb để lưu size process. Gán giá trị ban đầu là null. Nhớ khởi tạo *bm và *bmsem để sử dụng.

    ~PTable();
    //Hủy các đối tượng đã tạo.

    int ExecUpdate(char* name);  //return PID
    //Thực thi cho system call SC_EXEC, kiểm tra chương trình được gọi có tồn tại trong máy không. Kiểm tra thử xem chương trình gọi lại chính nó không? Chúng ta không cho phép điều này. Kiểm tra còn slot trống để lưu tiến trình mới không (max là 10 process). Nếu thỏa các điều kiện trên thì ta lấy index của slot trống là processID cảu tiền trình mới tạo này, giả sử là ID. Và gọi phương thức EXEC của lớp PCB với đối tượng tương ứng quản lý process này, nghĩa là gọi pcb[ID]->Exec(…). Xem chi tiết mo tả trong lớp PCB ở bên dưới.

    int ExitUpdate(int ec);

    int JoinUpdate(int id);
    //Được sử dụng cho system call Join(), trong thủ tục này thì tiến trình cha goi pcb[i]->JoinWait() để chờ cho tới khi tiến trình con kết thúc (trước khi tiến trình con kết thúc thì tiến trình con gọi JoinRelease()). Sau đó tiến trình cha gọi ExitRelease() để cho phép tiến trình con được kết thúc.

    int GetFreeSlot();
    //Tìm free slot để lưu thông tin cho tiến trình mới.

    bool IsExist(int pid);
    //Kiểm tra có tồn tại processId này không

    void Remove(int pid);
    //Xóa một processID ra khỏng mãng quản lý nó, khi mà tiến trình này kết thúc.

    char* GetFileName(int id);  // Trả về tên của tiến trình

    PCB* GetProcess(int id);
};
#endif
