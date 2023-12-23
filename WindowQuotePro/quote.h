#ifndef quote_h
#define quote_h
#include <string>

struct Window {
   std::string material;
   std::string size;
   Window(): material(""), size("") {}
   Window(const std::string& _material, const std::string& _size): material(_material), size(_size) {}

};

class Quote {
private:
   
   std::string name;
   std::string customer;
   Window window;
   double price;

public:
   Quote();
   ~Quote();
   Quote(const std::string& _name, const std::string& _customer, const Window& _window, double& _price);
   Quote(const Quote& other);
   Quote& operator=(const Quote& other);

   const std::string& GetName() const;
   const std::string& GetCustomer() const;
   const Window& GetWindow() const;
   const double Quote::GetPrice() const;

};

#endif //quote_h