/**
 * See Klaus Iglberger 'Breaking Dependencies - C++ Type Erasure - The Implementation Details',
 * CppCon 2022
 * https://www.youtube.com/watch?v=qn6OqefuH08
*/

#include <utility> 
#include <memory>

class Shape {
    private:
    // Interface (plays role of base class)
    struct ShapeConcept {
        virtual ~ShapeConcept() = default;
        virtual void do_serialize() const = 0;
        virtual void do_draw() const = 0;
        virtual std::unique_ptr<ShapeConcept> clone() const = 0;
    };

    template< typename ShapeT >
    struct ShapeModel: public ShapeConcept {
        ShapeModel( ShapeT shape )
        : shape_{ std::move(shape)}
        {}

        std::unique_ptr<ShapeConcept> clone() const override {
            return std::make_unique<ShapeModel>( *this );
        }

        void do_serialize() const override {
            serialize( shape_ );
        }

        void do_draw() const override {
            draw( shape_ );
        }

        ShapeT shape_;

    };

    friend void serialize( Shape const& shape )
    {
        shape.pimpl->do_serialize();
    }

    friend void draw( Shape const& shape )
    {
        shape.pimpl->do_draw();
    }

    std::unique_ptr<ShapeConcept> pimpl; 

    public: 
    template < typename ShapeT >
    Shape( ShapeT shape ) 
    : pimpl( std::make_unique<ShapeModel<ShapeT>>( std::move(shape) ) )
    {}

    // Copy operations
    Shape( Shape const& other )
    : pimpl(other.pimpl->clone())
    {}

    Shape& operator=(Shape const& other)
    {
        // Copy and swap idiom. Tmp goes out of scope. 
        Shape tmp( other );
        std::swap( pimpl, tmp.pimpl );
        // Another version would be: 
        // other.pimpl->clone().swap( pimpl); 
        return *this;
    }

    // Move operations
    Shape& operator=(Shape&& other ) noexcept
    {
        pimpl.swap( other.pimpl );
        return *this;
    }

};

class Circle{
    private:
    double radius{0};
    
    public:
    Circle(double radius_)
    :radius{ radius_ }
    {}

    double getRadius() const {
        return radius;
    }
};

class Square{
    private:
    double side{0};
    
    public:
    Square(double side_)
    :side{ side_ }
    {}

    double getSide() const {
        return side;
    }
};

#include <iostream>

void serialize(Circle const& c) {
    std::cout << "Serializing Circle: " << c.getRadius() << std::endl; 
}
void draw(Circle const& c) {
    std::cout << "Drawing Circle: " << c.getRadius() << std::endl; 
}
void serialize(Square const& s) {
    std::cout << "Serializing Square: " << s.getSide() << std::endl; 
}
void draw(Square const& s) {
    std::cout << "Drawing Square: " << s.getSide() << std::endl; 
}

void drawAllShapes( std::vector<Shape> const& shapes ){
    for( auto const & shape : shapes) {
        draw( shape );
    }
} 

int main()
{
    using Shapes = std::vector<Shape>; 

    // Create some shapes
    Shapes shapes; 
    shapes.emplace_back( Circle{ 2.0 } );
    shapes.emplace_back( Circle{ 3.0 } );
    shapes.emplace_back( Square{ 2.0 } );

    drawAllShapes( shapes );

    return 0;
}