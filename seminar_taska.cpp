#include <iostream>
#include <compare>

using namespace std;

class BigNumber {
    private:
        std::string* str;
        size_t       point;
        static std::strong_ordering compareByLen(size_t lena, size_t lenb) {
            if (lena > lenb)
                return std::strong_ordering::less;
            if (lena < lenb)
                return std::strong_ordering::greater;
            return std::strong_ordering::equivalent;
        }
        static std::strong_ordering compareDigits(const std::string* a, const std::string* b, 
                const size_t offset, const size_t length) {
            for (size_t i = offset; i < length; i++)
            {
                if (a->at(i) > b->at(i))
                    return std::strong_ordering::less;
                if (a->at(i) < b->at(i))
                    return std::strong_ordering::greater;
            }
            return std::strong_ordering::equivalent;
        }
        static bool checkNotAllNulls(const std::string* str, const size_t size, const size_t offset) {
            if (offset < size) {
                size_t endl = size;
                do {
                    if (str->at(endl) != '0')
                        return true;
                } while (--endl >= offset);
            }
            return false;
        }
        static std::strong_ordering basicCompare(const std::string* left, const std::string* right, const size_t leftPoint, const size_t rightPoint) {
            const size_t lena = left->size();
            const size_t lenb = right->size();
            std::strong_ordering result = std::strong_ordering::equivalent;
            if (leftPoint != std::string::npos && rightPoint != std::string::npos) {
                result = compareByLen(leftPoint, rightPoint);
            } else if (leftPoint == std::string::npos && rightPoint == std::string::npos) {
                result = compareByLen(lena, lenb);
                if (result == std::strong_ordering::equivalent)
                    return compareDigits(left, right, 0, lena); // lena == lenb
            } else {
                if (leftPoint == std::string::npos) {
                    result = compareByLen(lena, rightPoint);
                } else if (rightPoint == std::string::npos) {
                    result = compareByLen(lenb, leftPoint);
                    if (result == std::strong_ordering::equivalent) {
                        return result == std::strong_ordering::greater 
                            ? std::strong_ordering::less 
                            : std::strong_ordering::greater;
                    }
                }
            }
            return result;
        }
        std::strong_ordering tailCompare(const BigNumber* comparable, const size_t shortestLength) const {
            bool result = checkNotAllNulls(this->getString(), this->str->size() - 1, shortestLength);
            if (result == false) {
                result = checkNotAllNulls(comparable->getString(), comparable->getString()->size() - 1, shortestLength);
                if (result)
                    return std::strong_ordering::greater;
            } else {
                return std::strong_ordering::less;
            }
            return std::strong_ordering::equal;
        }
    public:
        BigNumber() {
            this->str = nullptr;
            this->point = std::string::npos;
        }
        BigNumber(std::string* a) {
            this->str = a;
            this->point = a->find_first_of(",");
        }
        ~BigNumber() {
        }
        const std::string* getString() const {
            return const_cast<std::string*>(this->str);
        }
        const size_t getPointPosition() const {
            return this->point;
        }
        std::strong_ordering operator <=>(const BigNumber &b) const {
            std::cout << "wtf" << endl;
            // std::partial_ordering result = basicCompare(b.getString(), b.getPointPosition());
            std::strong_ordering result = basicCompare(this->str, b.getString(), this->point, b.getPointPosition());
            if (result != std::strong_ordering::equivalent)
                return result;
            // digita == digitb
            size_t lena = this->str->size();
            size_t lenb = b.getString()->size();
            size_t digita = this->point;
            result = compareDigits(this->str, b.getString(), 0, digita);
            if (result != std::strong_ordering::equivalent)
                return result;
            // ok, first part eq
            size_t safeLength = (lena > lenb ? lenb : lena);
            result = compareDigits(this->str, b.getString(), digita + 1, safeLength);
            if (result != std::strong_ordering::equivalent)
                return result;
            return tailCompare(&b, safeLength);

        }
};

int main()
{
    std::string a;
    std::string b;
    cin >> a >> b;
    auto left = BigNumber(&a);
    auto right = BigNumber(&b);
    auto res = left <=> right;
    if (res == std::strong_ordering::equivalent) {
        cout << "==" << endl;
    } else if (res == std::strong_ordering::less) {
        cout << ">" << endl;
    } else if (res == std::strong_ordering::greater) {
            cout << "<" << endl;
    } else {
        cout << "error" << endl;
    }
    return 0;
}