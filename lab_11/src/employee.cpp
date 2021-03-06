#include "employees.h"
#include <algorithm>

#include <iostream>
void str_to_c(char* dest, std::string src) {
    for (int i = 0; i < (int)src.size(); ++i) {
        dest[i] = src[i];
    }
    dest[src.size()] = '\0';
}

Employee::~Employee() {
    delete[] _name;
}

std::ostream & Employee::file_output(std::ostream &os) const{
    return os;
}

std::ostream &operator<<(std::ostream &os, const Employee &emp) {
    return emp.file_output(os);
}

std::istream &operator>>(std::istream &ios, Developer &dev) {
    std::string tmp;
    ios >> tmp >> dev._base_salary;
    dev._name = new char[tmp.size() + 1];
    str_to_c(dev._name, tmp);
    ios >> dev._has_bonus;
    return ios;
}

std::ostream &operator<<(std::ostream &os, const Developer &dev) {
    int type = 1;
    os.write((char*)&type, sizeof(int32_t));
    os.write(dev._name, strlen(dev._name) + 1);
    os.write((char*)&dev._base_salary, sizeof(int32_t));
    os.write((char*)&dev._has_bonus, sizeof(bool));
    return os;
}

std::ostream &operator<<(std::ostream &os, const SalesManager &seller) {
    int type = 2;
    os.write((char*)&type, sizeof(int32_t));
    os.write(seller._name, strlen(seller._name) + 1);
    os.write((char*)&seller._base_salary, sizeof(int32_t));
    os.write((char*)&seller._sold_nm, sizeof(int32_t));
    os.write((char*)&seller._price, sizeof(int32_t));
    return os;
}

std::istream &operator>>(std::istream &ios, SalesManager &seller) {
    std::string tmp;
    ios >> tmp >> seller._base_salary;
    seller._name = new char[tmp.size() + 1];
    str_to_c(seller._name, tmp);
    ios >> seller._sold_nm >> seller._price;
    return ios;
}

std::ifstream &operator>>(std::ifstream &ios, Developer &dev) {
    std::string tmp;
    std::getline(ios, tmp, '\0');
    dev._name = new char[tmp.size() + 1];
    str_to_c(dev._name, tmp);
    ios.read((char*)&dev._base_salary, sizeof(int32_t));
    ios.read((char*)&dev._has_bonus, sizeof(bool));
    return ios;

}
std::ifstream &operator>>(std::ifstream &ios, SalesManager &seller) {
    std::string tmp;
    std::getline(ios, tmp, '\0');
    seller._name = new char[tmp.size() + 1];
    str_to_c(seller._name, tmp);
    ios.read((char*)&seller._base_salary, sizeof(int32_t));
    ios.read((char*)&seller._sold_nm, sizeof(int32_t));
    ios.read((char*)&seller._price, sizeof(int32_t));
    return ios;
}

void Developer::term_output(std::ostream& os) const{
    os << "Developer\n";
    printf("Name: %s\n", _name);
    os << "Base Salary: " << _base_salary << '\n';
    os << "Has bonus: " << ((_has_bonus) ? '+' : '-' )<< '\n';

}

std::ostream & Developer::file_output(std::ostream &os) const{
    os << *this;
    return os;
}


void SalesManager::term_output(std::ostream& os) const {
    os << "Sales Manager\n";
    printf("Name: %s\n", _name);
    os << "Base Salary: " << _base_salary << '\n';
    os << "Sold items: " << _sold_nm << '\n';
    os << "Item price: " << _price << '\n';
}

std::ostream & SalesManager::file_output(std::ostream &os) const{
    os << *this;
    return os;
}


EmployeesArray::EmployeesArray() {
    _size = _capacity = 0;
}

void EmployeesArray::add(Employee *e) {
    if (_capacity == 0) {
        _capacity = 1;
        _employees = new Employee*[1];
    }
    if (_capacity == _size) {
        _capacity *= 2;
        Employee** tmp = new Employee*[_capacity];
        for (int i = 0; i < _size; ++i) {
            tmp[i] = _employees[i];
        }
        std::swap(tmp, _employees);
        delete[] tmp;
    }
    _employees[_size] = e;
    _size++;
}

int EmployeesArray::total_salary() const {
    int total = 0;
    for (int i = 0; i < _size; ++i) {
        total += _employees[i]->salary();
    }
    return total;
}

EmployeesArray::~EmployeesArray() {
    if (_size == 0)
        return;
    for (int i = 0; i < _size; ++i) {
        delete _employees[i];
    }
    delete[] _employees;
}

std::ostream &operator<<(std::ostream &os, const EmployeesArray& array) {
    for (int i = 0; i < array._size; ++i) {
        os << i + 1 << ". ";
        array._employees[i]->term_output(os);
    }
    os << "== Total salary: " << array.total_salary() << '\n';
    return os;
}


std::ofstream &operator<<(std::ofstream &os, const EmployeesArray& array) {
    os.write((char*)&array._size, sizeof(int32_t));
    for (int i = 0; i < array._size; ++i) {
        os << *array._employees[i];
    }
    return os;
}


std::ifstream &operator>>(std::ifstream &ios, EmployeesArray& array) {
    int size = 0;
    ios.read((char*)&size, sizeof(int32_t));
    for (int i = 0; i < size; ++i) {
        int type = 0;
        ios.read((char*)&type, sizeof(int32_t));
        if (type == 1) {
            Developer* dev = new Developer;
            ios >> *dev;
            array.add(dev);
        } else if (type == 2) {
            SalesManager* seller = new SalesManager;
            ios >> *seller;
            array.add(seller);
        }
    }
    return ios;
}
