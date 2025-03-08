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
    int maLopTC;     
    char maMH[11];   
    char nienKhoa[10]; 
    int hocKy;       
    int nhom;        
    int soSVMin;     
    int soSVMax;     
    bool huyLop;     
    DangKy* dsSVDK;  
};

// Cấu trúc lớp sinh viên (danh sách tuyến tính cấp phát động)
struct Lop {
    char maLop[16];  // MALOP (C15)
    char tenLop[51]; // TENLOP
    SinhVien* dsSV;  // Danh sách sinh viên
};


// =============== KHAI BÁO DỮ LIỆU TOÀN CỤC ===============
