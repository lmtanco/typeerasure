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
            resturn std::make_unique<ShapeModel>(*this);
        }

        void do_serialize() const override {
            serialize(shape_);
        }

        void do_draw() const override {
            draw(shape_);
        }

        ShapeT shape_;

    };

    friend void serialize( Shape const& shape)
    {
        shape.pimpl->do_serialize();
    }

    friend void draw( Shape const& shape)
    {
        shape.pimpl->do_draw();
    }

    std::unique_ptr<ShapeConcept> pimpl; 

    public: 
    template < typename ShapeT >
    Shape( ShapeT shape ) 
    : pimpl(std::make_unique<ShapeModel<ShapeT>>(std::move(shape)))
    {}

    // Copy 

};



int main()
{
    return 0;
}