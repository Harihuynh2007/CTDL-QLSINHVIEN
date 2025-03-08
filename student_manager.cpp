#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
#include <algorithm>

using namespace std;

// =============== ĐỊNH NGHĨA CẤU TRÚC DỮ LIỆU ===============

// Cấu trúc môn học (cây nhị phân tìm kiếm)

struct MonHoc{
    char maMH[11];
    char tenMH[51];
    int soTCLT; // SO TIN CHI LY THUYET
    int soTCTH; // SO TIN CHI THUC HANH
    MonHoc *left;
    MonHoc *right;
};

// Cấu trúc sinh viên (danh sách liên kết đơn)

struct SinhVien{
    char maSV[11];
    char ho[31];
    char ten[11];
    char phai[4];
    char soDT[12];
    SinhVien* next;
};

// Cấu trúc đăng ký (danh sách liên kết đơn)
struct DangKy
{
    char maSV[16];
    float diem;
    DangKy* next;
};

// Cấu trúc lớp tín chỉ (danh sách tuyến tính con trỏ)
struct LopTinChi {
    int maLopTC;     // MALOPTC (tự động tăng)
    char maMH[11];   // MAMH
    char nienKhoa[10]; // Niên khóa
    int hocKy;       // Học kỳ
    int nhom;        // Nhóm
    int soSVMin;     // Số SV tối thiểu
    int soSVMax;     // Số SV tối đa
    bool huyLop;     // Trạng thái hủy lớp
    DangKy* dsSVDK;  // Danh sách SV đăng ký
};
