#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
#include <algorithm>

using namespace std;

// =============== ĐỊNH NGHĨA CẤU TRÚC DỮ LIỆU ===============

// Cấu trúc môn học (cây nhị phân tìm kiếm)
struct MonHoc {
    char maMH[11];   // MAMH (C10)
    char tenMH[51];  // TENMH (C50)
    int soTCLT;      // Số tín chỉ lý thuyết
    int soTCTH;      // Số tín chỉ thực hành
    MonHoc* left;
    MonHoc* right;
};

// Cấu trúc sinh viên (danh sách liên kết đơn)
struct SinhVien {
    char maSV[16];   // MASV (C15)
    char ho[31];     // HO
    char ten[11];    // TEN
    char phai[4];    // PHAI
    char soDT[12];   // SODT
    SinhVien* next;
};

// Cấu trúc đăng ký (danh sách liên kết đơn)
struct DangKy {
    char maSV[16];   // MASV (C15)
    float diem;      // DIEM
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

// Cấu trúc lớp sinh viên (danh sách tuyến tính cấp phát động)
struct Lop {
    char maLop[16];  // MALOP (C15)
    char tenLop[51]; // TENLOP
    SinhVien* dsSV;  // Danh sách sinh viên
};

// =============== KHAI BÁO DỮ LIỆU TOÀN CỤC ===============

MonHoc* rootMonHoc = NULL;         // Cây nhị phân môn học
Lop* dsLop = NULL;                 // DS lớp cấp phát động
int soLuongLop = 0;                // Số lượng lớp hiện tại
int kichThuocDSLop = 0;            // Kích thước mảng cấp phát
LopTinChi* dsLopTC = NULL;         // DS lớp tín chỉ
int soLuongLopTC = 0;              // Số lượng lớp tín chỉ hiện tại
const int MAX_LOP_TC = 10000;      // Số lượng lớp tín chỉ tối đa

// =============== CHỨC NĂNG HỖ TRỢ ===============

// Hàm so sánh hai chuỗi, không phân biệt hoa thường
int strcmpi(const char* s1, const char* s2) {
    char c1, c2;
    while ((c1 = tolower(*s1)) == (c2 = tolower(*s2))) {
        if (*s1 == '\0') return 0;
        s1++;
        s2++;
    }
    return c1 - c2;
}

// Hàm tìm kiếm sinh viên theo mã sinh viên
SinhVien* timSinhVien(const char* maSV) {
    for (int i = 0; i < soLuongLop; i++) {
        SinhVien* p = dsLop[i].dsSV;
        while (p != NULL) {
            if (strcmp(p->maSV, maSV) == 0) return p;
            p = p->next;
        }
    }
    return NULL;
}

// Hàm tìm kiếm lớp theo mã lớp
int timLop(const char* maLop) {
    for (int i = 0; i < soLuongLop; i++) {
        if (strcmp(dsLop[i].maLop, maLop) == 0) return i;
    }
    return -1;
}

// Hàm tìm kiếm môn học theo mã môn học
MonHoc* timMonHoc(MonHoc* root, const char* maMH) {
    if (root == NULL) return NULL;
    
    int cmp = strcmp(maMH, root->maMH);
    if (cmp == 0) return root;
    if (cmp < 0) return timMonHoc(root->left, maMH);
    return timMonHoc(root->right, maMH);
}

// Tìm lớp tín chỉ theo mã lớp tín chỉ
int timLopTC(int maLopTC) {
    for (int i = 0; i < soLuongLopTC; i++) {
        if (dsLopTC[i].maLopTC == maLopTC) return i;
    }
    return -1;
}

// Hàm tính chiều cao của cây
int chieuCao(MonHoc* root) {
    if (root == NULL) return 0;
    int chieuCaoTrai = chieuCao(root->left);
    int chieuCaoPhai = chieuCao(root->right);
    return (chieuCaoTrai > chieuCaoPhai ? chieuCaoTrai : chieuCaoPhai) + 1;
}

// Hàm xoay phải để cân bằng cây
MonHoc* xoayPhai(MonHoc* y) {
    MonHoc* x = y->left;
    MonHoc* T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    return x;
}

// Hàm xoay trái để cân bằng cây
MonHoc* xoayTrai(MonHoc* x) {
    MonHoc* y = x->right;
    MonHoc* T2 = y->left;
    
    y->left = x;
    x->right = T2;
    
    return y;
}

// Hàm lấy hệ số cân bằng của một nút
int heSoCanBang(MonHoc* N) {
    if (N == NULL) return 0;
    return chieuCao(N->left) - chieuCao(N->right);
}

// Hàm thêm môn học vào cây cân bằng
MonHoc* themMonHoc(MonHoc* node, const char* maMH, const char* tenMH, int soTCLT, int soTCTH) {
    // 1. Thêm nút như BST thông thường
    if (node == NULL) {
        MonHoc* newNode = new MonHoc;
        strcpy(newNode->maMH, maMH);
        strcpy(newNode->tenMH, tenMH);
        newNode->soTCLT = soTCLT;
        newNode->soTCTH = soTCTH;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }
    
    int cmp = strcmp(maMH, node->maMH);
    if (cmp < 0)
        node->left = themMonHoc(node->left, maMH, tenMH, soTCLT, soTCTH);
    else if (cmp > 0)
        node->right = themMonHoc(node->right, maMH, tenMH, soTCLT, soTCTH);
    else // Mã môn học đã tồn tại, cập nhật thông tin
    {
        strcpy(node->tenMH, tenMH);
        node->soTCLT = soTCLT;
        node->soTCTH = soTCTH;
        return node;
    }
    
    // 2. Cập nhật hệ số cân bằng và cân bằng cây
    int balance = heSoCanBang(node);
    
    // Trường hợp Left Left
    if (balance > 1 && strcmp(maMH, node->left->maMH) < 0)
        return xoayPhai(node);
    
    // Trường hợp Right Right
    if (balance < -1 && strcmp(maMH, node->right->maMH) > 0)
        return xoayTrai(node);
    
    // Trường hợp Left Right
    if (balance > 1 && strcmp(maMH, node->left->maMH) > 0) {
        node->left = xoayTrai(node->left);
        return xoayPhai(node);
    }
    
    // Trường hợp Right Left
    if (balance < -1 && strcmp(maMH, node->right->maMH) < 0) {
        node->right = xoayPhai(node->right);
        return xoayTrai(node);
    }
    
    return node;
}

// Tìm giá trị nhỏ nhất trong cây con
MonHoc* timNodeNhoNhat(MonHoc* node) {
    MonHoc* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// Hàm xóa một môn học khỏi cây
MonHoc* xoaMonHoc(MonHoc* root, const char* maMH) {
    if (root == NULL) return root;
    
    int cmp = strcmp(maMH, root->maMH);
    
    if (cmp < 0)
        root->left = xoaMonHoc(root->left, maMH);
    else if (cmp > 0)
        root->right = xoaMonHoc(root->right, maMH);
    else {
        // Nút có một hoặc không có con
        if (root->left == NULL) {
            MonHoc* temp = root->right;
            delete root;
            return temp;
        } else if (root->right == NULL) {
            MonHoc* temp = root->left;
            delete root;
            return temp;
        }
        
        // Nút có hai con
        MonHoc* temp = timNodeNhoNhat(root->right);
        strcpy(root->maMH, temp->maMH);
        strcpy(root->tenMH, temp->tenMH);
        root->soTCLT = temp->soTCLT;
        root->soTCTH = temp->soTCTH;
        
        root->right = xoaMonHoc(root->right, temp->maMH);
    }
    
    if (root == NULL) return root;
    
    // Cập nhật hệ số cân bằng và cân bằng cây
    int balance = heSoCanBang(root);
    
    // Trường hợp Left Left
    if (balance > 1 && heSoCanBang(root->left) >= 0)
        return xoayPhai(root);
    
    // Trường hợp Left Right
    if (balance > 1 && heSoCanBang(root->left) < 0) {
        root->left = xoayTrai(root->left);
        return xoayPhai(root);
    }
    
    // Trường hợp Right Right
    if (balance < -1 && heSoCanBang(root->right) <= 0)
        return xoayTrai(root);
    
    // Trường hợp Right Left
    if (balance < -1 && heSoCanBang(root->right) > 0) {
        root->right = xoayPhai(root->right);
        return xoayTrai(root);
    }
    
    return root;
}

// Đếm số sinh viên đã đăng ký trong lớp tín chỉ
int demSoSVDK(DangKy* dsDK) {
    int count = 0;
    DangKy* p = dsDK;
    while (p != NULL) {
        count++;
        p = p->next;
    }
    return count;
}

// Thêm sinh viên vào danh sách đăng ký
void themSVDK(DangKy*& dsDK, const char* maSV) {
    DangKy* p = new DangKy;
    strcpy(p->maSV, maSV);
    p->diem = -1; // Chưa có điểm
    p->next = dsDK;
    dsDK = p;
}

// Xóa sinh viên khỏi danh sách đăng ký
bool xoaSVDK(DangKy*& dsDK, const char* maSV) {
    if (dsDK == NULL) return false;
    
    if (strcmp(dsDK->maSV, maSV) == 0) {
        DangKy* temp = dsDK;
        dsDK = dsDK->next;
        delete temp;
        return true;
    }
    
    DangKy* p = dsDK;
    while (p->next != NULL && strcmp(p->next->maSV, maSV) != 0) {
        p = p->next;
    }
    
    if (p->next == NULL) return false;
    
    DangKy* temp = p->next;
    p->next = p->next->next;
    delete temp;
    return true;
}

// =============== CHỨC NĂNG CHÍNH ===============

// a. Mở lớp tín chỉ
void moLopTinChi() {
    if (soLuongLopTC >= MAX_LOP_TC) {
        cout << "Đã đạt số lượng lớp tín chỉ tối đa!" << endl;
        return;
    }
    
    if (dsLopTC == NULL) {
        dsLopTC = new LopTinChi[MAX_LOP_TC];
    }
    
    cout << "\n===== MỞ LỚP TÍN CHỈ =====\n";
    cout << "1. Thêm lớp tín chỉ" << endl;
    cout << "2. Xóa lớp tín chỉ" << endl;
    cout << "3. Hiệu chỉnh thông tin lớp tín chỉ" << endl;
    cout << "0. Trở về menu chính" << endl;
    cout << "Chọn chức năng: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    switch(choice) {
        case 1: {
            char maMH[11];
            char nienKhoa[10];
            int hocKy, nhom, soSVMin, soSVMax;
            
            cout << "Nhập mã môn học: ";
            cin.getline(maMH, 11);
            
            MonHoc* mh = timMonHoc(rootMonHoc, maMH);
            if (mh == NULL) {
                cout << "Môn học không tồn tại!" << endl;
                return;
            }
            
            cout << "Nhập niên khóa (vd: 2022-2023): ";
            cin.getline(nienKhoa, 10);
            
            cout << "Nhập học kỳ (1, 2, 3): ";
            cin >> hocKy;
            
            cout << "Nhập nhóm: ";
            cin >> nhom;
            
            cout << "Nhập số SV tối thiểu: ";
            cin >> soSVMin;
            
            cout << "Nhập số SV tối đa: ";
            cin >> soSVMax;
            
            // Kiểm tra trùng lặp
            bool trungLap = false;
            for (int i = 0; i < soLuongLopTC; i++) {
                if (strcmp(dsLopTC[i].maMH, maMH) == 0 && 
                    strcmp(dsLopTC[i].nienKhoa, nienKhoa) == 0 && 
                    dsLopTC[i].hocKy == hocKy && 
                    dsLopTC[i].nhom == nhom) {
                    trungLap = true;
                    break;
                }
            }
            
            if (trungLap) {
                cout << "Lớp tín chỉ này đã tồn tại!" << endl;
                return;
            }
            
            // Thêm lớp tín chỉ mới
            dsLopTC[soLuongLopTC].maLopTC = soLuongLopTC + 1; // Tự động tăng
            strcpy(dsLopTC[soLuongLopTC].maMH, maMH);
            strcpy(dsLopTC[soLuongLopTC].nienKhoa, nienKhoa);
            dsLopTC[soLuongLopTC].hocKy = hocKy;
            dsLopTC[soLuongLopTC].nhom = nhom;
            dsLopTC[soLuongLopTC].soSVMin = soSVMin;
            dsLopTC[soLuongLopTC].soSVMax = soSVMax;
            dsLopTC[soLuongLopTC].huyLop = false;
            dsLopTC[soLuongLopTC].dsSVDK = NULL;
            
            cout << "Đã mở lớp tín chỉ mã số " << dsLopTC[soLuongLopTC].maLopTC << " thành công!" << endl;
            soLuongLopTC++;
            break;
        }
        case 2: {
            int maLopTC;
            cout << "Nhập mã lớp tín chỉ cần xóa: ";
            cin >> maLopTC;
            
            int viTri = timLopTC(maLopTC);
            if (viTri == -1) {
                cout << "Lớp tín chỉ không tồn tại!" << endl;
                return;
            }
            
            // Xóa danh sách sinh viên đăng ký
            DangKy* p = dsLopTC[viTri].dsSVDK;
            while (p != NULL) {
                DangKy* temp = p;
                p = p->next;
                delete temp;
            }
            
            // Dịch chuyển các phần tử để xóa
            for (int i = viTri; i < soLuongLopTC - 1; i++) {
                dsLopTC[i] = dsLopTC[i + 1];
            }
            
            soLuongLopTC--;
            cout << "Đã xóa lớp tín chỉ thành công!" << endl;
            break;
        }
        case 3: {
            int maLopTC;
            cout << "Nhập mã lớp tín chỉ cần hiệu chỉnh: ";
            cin >> maLopTC;
            cin.ignore();
            
            int viTri = timLopTC(maLopTC);
            if (viTri == -1) {
                cout << "Lớp tín chỉ không tồn tại!" << endl;
                return;
            }
            
            cout << "Thông tin hiện tại của lớp tín chỉ:" << endl;
            cout << "Mã môn học: " << dsLopTC[viTri].maMH << endl;
            cout << "Niên khóa: " << dsLopTC[viTri].nienKhoa << endl;
            cout << "Học kỳ: " << dsLopTC[viTri].hocKy << endl;
            cout << "Nhóm: " << dsLopTC[viTri].nhom << endl;
            cout << "Số SV tối thiểu: " << dsLopTC[viTri].soSVMin << endl;
            cout << "Số SV tối đa: " << dsLopTC[viTri].soSVMax << endl;
            
            char maMH[11];
            char nienKhoa[10];
            int hocKy, nhom, soSVMin, soSVMax;
            
            cout << "\nNhập thông tin mới (Nhấn Enter để giữ nguyên):" << endl;
            
            cout << "Nhập mã môn học: ";
            cin.getline(maMH, 11);
            if (strlen(maMH) > 0) {
                MonHoc* mh = timMonHoc(rootMonHoc, maMH);
                if (mh == NULL) {
                    cout << "Môn học không tồn tại!" << endl;
                    return;
                }
                strcpy(dsLopTC[viTri].maMH, maMH);
            }
            
            cout << "Nhập niên khóa: ";
            cin.getline(nienKhoa, 10);
            if (strlen(nienKhoa) > 0) {
                strcpy(dsLopTC[viTri].nienKhoa, nienKhoa);
            }
            
            cout << "Nhập học kỳ (1, 2, 3): ";
            string input;
            getline(cin, input);
            if (!input.empty()) {
                hocKy = stoi(input);
                dsLopTC[viTri].hocKy = hocKy;
            }
            
            cout << "Nhập nhóm: ";
            getline(cin, input);
            if (!input.empty()) {
                nhom = stoi(input);
                dsLopTC[viTri].nhom = nhom;
            }
            
            cout << "Nhập số SV tối thiểu: ";
            getline(cin, input);
            if (!input.empty()) {
                soSVMin = stoi(input);
                dsLopTC[viTri].soSVMin = soSVMin;
            }
            
            cout << "Nhập số SV tối đa: ";
            getline(cin, input);
            if (!input.empty()) {
                soSVMax = stoi(input);
                dsLopTC[viTri].soSVMax = soSVMax;
            }
            
            cout << "Đã cập nhật thông tin lớp tín chỉ thành công!" << endl;
            break;
        }
        case 0:
            return;
        default:
            cout << "Lựa chọn không hợp lệ!" << endl;
    }
}

// b. In danh sách sinh viên đã đăng ký lớp tín chỉ
void inDSSVDangKy() {
    char nienKhoa[10];
    int hocKy, nhom;
    char maMH[11];
    
    cout << "\n===== IN DANH SÁCH SINH VIÊN ĐĂNG KÝ LỚP TÍN CHỈ =====\n";
    cout << "Nhập niên khóa: ";
    cin.getline(nienKhoa, 10);
    
    cout << "Nhập học kỳ: ";
    cin >> hocKy;
    cin.ignore();
    
    cout << "Nhập mã môn học: ";
    cin.getline(maMH, 11);
    
    cout << "Nhập nhóm: ";
    cin >> nhom;
    cin.ignore();
    
    // Tìm lớp tín chỉ
    int viTri = -1;
    for (int i = 0; i < soLuongLopTC; i++) {
        if (strcmp(dsLopTC[i].maMH, maMH) == 0 && 
            strcmp(dsLopTC[i].nienKhoa, nienKhoa) == 0 && 
            dsLopTC[i].hocKy == hocKy && 
            dsLopTC[i].nhom == nhom) {
            viTri = i;
            break;
        }
    }
    
    if (viTri == -1) {
        cout << "Không tìm thấy lớp tín chỉ phù hợp!" << endl;
        return;
    }
    
    // Tìm thông tin môn học
    MonHoc* mh = timMonHoc(rootMonHoc, maMH);
    if (mh == NULL) {
        cout << "Không tìm thấy thông tin môn học!" << endl;
        return;
    }
    
    cout << "\nDANH SÁCH SINH VIÊN ĐĂNG KÝ LỚP TÍN CHỈ" << endl;
    cout << "Môn học: " << mh->tenMH << " (" << maMH << ")" << endl;
    cout << "Niên khóa: " << nienKhoa << " - Học kỳ: " << hocKy << " - Nhóm: " << nhom << endl;
    cout << "Mã lớp TC: " << dsLopTC[viTri].maLopTC << endl;
    cout << "\n";
    cout << setw(5) << "STT" << setw(16) << "Mã SV" << setw(31) << "Họ" << setw(11) << "Tên" << setw(16) << "Lớp" << endl;
    cout << string(79, '-') << endl;
    
    DangKy* dk = dsLopTC[viTri].dsSVDK;
    int stt = 1;
    
    while (dk != NULL) {
        SinhVien* sv = timSinhVien(dk->maSV);
        if (sv != NULL) {
            // Tìm lớp của sinh viên
            string tenLop = "";
            for (int i = 0; i < soLuongLop; i++) {
                SinhVien* p = dsLop[i].dsSV;
                while (p != NULL) {
                    if (strcmp(p->maSV, dk->maSV) == 0) {
                        tenLop = dsLop[i].tenLop;
                        break;
                    }
                    p = p->next;
                }
                if (!tenLop.empty()) break;
            }
            
            cout << setw(5) << stt++ << setw(16) << sv->maSV << setw(31) << sv->ho << setw(11) << sv->ten << setw(16) << tenLop << endl;
        }
        dk = dk->next;
    }
    
    if (stt == 1) {
        cout << "Chưa có sinh viên đăng ký lớp tín chỉ này!" << endl;
    }
}

// c. Nhập lớp sinh viên
void nhapLopSinhVien() {
    cout << "\n===== NHẬP LỚP SINH VIÊN =====\n";
    cout << "1. Thêm lớp mới" << endl;
    cout << "2. Xóa lớp" << endl;
    cout << "3. Hiệu chỉnh thông tin lớp" << endl;
    cout << "4. Nhập sinh viên vào lớp" << endl;
    cout << "0. Trở về menu chính" << endl;
    cout << "Chọn chức năng: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    switch(choice) {
        case 1: {
            // Nếu mảng đã đầy, cấp phát thêm
            if (soLuongLop >= kichThuocDSLop) {
                int newSize = (kichThuocDSLop == 0) ? 10 : kichThuocDSLop * 2;
                Lop* newDSLop = new Lop[newSize];
                
                // Sao chép dữ liệu cũ sang mảng mới
                for (int i = 0; i < soLuongLop; i++) {
                    newDSLop[i] = dsLop[i];
                }
                
                // Giải phóng mảng cũ và cập nhật con trỏ
                if (dsLop != NULL) delete[] dsLop;
                dsLop = newDSLop;
                kichThuocDSLop = newSize;
            }
            
            char maLop[16];
            char tenLop[51];
            
            cout << "Nhập mã lớp: ";
            cin.getline(maLop, 16);
            
            // Kiểm tra trùng lặp
            if (timLop(maLop) != -1) {
                cout << "Mã lớp đã tồn tại!" << endl;
                return;
            }
            
            cout << "Nhập tên lớp: ";
            cin.getline(tenLop, 51);
            
            // Thêm lớp mới
            strcpy(dsLop[soLuongLop].maLop, maLop);
            strcpy(dsLop[soLuongLop].tenLop, tenLop);
            dsLop[soLuongLop].dsSV = NULL;
            
            cout << "Đã thêm lớp " << maLop << " - " << tenLop << " thành công!" << endl;
            soLuongLop++;
            break;
        }
        case 2: {
            char maLop[16];
            cout << "Nhập mã lớp cần xóa: ";
            cin.getline(maLop, 16