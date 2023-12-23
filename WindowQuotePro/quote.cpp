#include "quote.h"

// Quote
Quote::Quote()
{
}

Quote::~Quote()
{
}

Quote::Quote(const std::string& _name, const std::string& _customer, const Window& _window, double& _price)
: name(_name)
, customer(_customer)
, window(_window)
, price(_price)
{
}

Quote::Quote(const Quote& other)
: name(other.name)
, customer(other.customer)
, window(other.window)
, price(other.price)
{
}

Quote& Quote::operator=(const Quote& other)
{
   if (this != &other) { 
      name = other.name;
      customer = other.customer;
      window = other.window;
      price = other.price;
   }
   return *this;
}

const std::string& Quote::GetName() const
{
   return name;
}

const std::string& Quote::GetCustomer() const
{
   return customer;
}

const Window& Quote::GetWindow() const
{
   return window;
}

const double Quote::GetPrice() const
{
   return price;
}
