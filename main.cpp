#include <iostream>
#include <memory>

/// Parts of guitar

class Strings
{
    std::string string_name;
    public:
        std::string get_name()
        {
                return string_name;
        }

        void set_name(std::string new_string_name)
        {
            this->string_name = new_string_name;
        }
};

class Body
{
    std::string body_name;
    public:
        std::string get_name()
        {
            return body_name;
        }

        void set_name(std::string new_body_name)
        {
            this->body_name = new_body_name;
        }
};

/// Abstract of builder

class Builder
{
    public:
        virtual std::shared_ptr<Body> get_body() = 0;
        virtual std::shared_ptr<Strings> get_strings() = 0;
};

/// Visual. AbstractGuitar could be created. Beware this.

class AbstractGuitar
{
    std::shared_ptr<Body> body;
    std::shared_ptr<Strings> strings;
    public:
        friend class Director;
        void specification()
        {
            std::cout << "Guitar's body " << body->get_name() << '\n';
            std::cout << "Guitar's strings name: " << strings->get_name() << '\n';
            std::cout << '\n';
        }
};

/// Logic

class Director
{   std::shared_ptr<Builder> builder;
    public:
        template<typename B>
        void set_builder(B new_builder)
        {
                if(!std::is_base_of<Builder, B>())
                    throw "Wrong object's type";

            this->builder = std::make_shared<B>();
        }

        std::shared_ptr<AbstractGuitar> get_abstract_guitar()
        {
            std::shared_ptr<AbstractGuitar> new_guitar(new AbstractGuitar());
            new_guitar->body = builder->get_body();
            new_guitar->strings = builder->get_strings();
            return new_guitar;
        }
};

/// Namespace to avoid name collisions

namespace automate
{
    template<typename T>
    std::shared_ptr<T> automate_guitar_set(std::string body_name)
    {
        std::shared_ptr<T> new_sth(new T());
        new_sth->set_name(body_name);
        return new_sth;
    }
    template<typename B>
    void automate_create_builder(B builder, Director director, std::shared_ptr<AbstractGuitar> abstract_guitar)
    {
        director.set_builder(builder);
        abstract_guitar = director.get_abstract_guitar();
        abstract_guitar->specification();
    }
}

/// Concrete objects

class FirstGuitar: public Builder
{
    public:
        std::shared_ptr<Body> get_body()
        {
            return automate::automate_guitar_set<Body>("First super guitar");
        }

        std::shared_ptr<Strings> get_strings()
        {
            return automate::automate_guitar_set<Strings>("First guitar string");
        }
};

class SecondGuitar: public Builder
{
    public:
        std::shared_ptr<Body> get_body()
        {
            return automate::automate_guitar_set<Body>("Second super guitar");
        }
        std::shared_ptr<Strings> get_strings()
        {
            return automate::automate_guitar_set<Strings>("Second super guitar string!");
        }
};

class ThirdGuitar: public Builder
{
    public:
        std::shared_ptr<Body> get_body()
        {
            return automate::automate_guitar_set<Body>("Third guitar");
        }

        std::shared_ptr<Strings> get_strings()
        {
            return automate::automate_guitar_set<Strings>("Third super guitar string!");
        }
};
int main()
{
    Director director;
    std::shared_ptr<AbstractGuitar> general_abstract_guitar(new AbstractGuitar());
    FirstGuitar first;
    SecondGuitar second;
    ThirdGuitar third;
    std::weak_ptr<AbstractGuitar> weak_to_check = general_abstract_guitar;
    automate::automate_create_builder(first, director, general_abstract_guitar);
    automate::automate_create_builder(second, director, general_abstract_guitar);
    automate::automate_create_builder(third, director, general_abstract_guitar);
    automate::automate_create_builder(first, director, general_abstract_guitar);
    if(weak_to_check.lock())
        std::cout << "Everything went ok.\n";
}
