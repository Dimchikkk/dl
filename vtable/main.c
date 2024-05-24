#include <stdio.h>

// Define the Shape interface
typedef struct {
    double (*area)();
    double (*perimeter)();
} Shape;

// Define the Circle type
typedef struct {
    Shape base;
    double radius;
} Circle;

// Method implementations for Circle
double circle_area(Circle *self) {
    return 3.14 * self->radius * self->radius;
}

double circle_perimeter(Circle *self) {
    return 2 * 3.14 * self->radius;
}

// Define the Square type
typedef struct {
    Shape base;
    double side;
} Square;

// Method implementations for Square
double square_area(Square *self) {
    return self->side * self->side;
}

double square_perimeter(Square *self) {
    return 4 * self->side;
}

int main() {
    // Create instances of Circle and Square
    Circle circle = {{circle_area, circle_perimeter}, 5.0};
    Square square = {{square_area, square_perimeter}, 4.0};

    // Use the virtual table to call methods polymorphically
    printf("Circle:\n");
    printf("Area: %.2f\n", (*(circle.base.area))(&circle));
    printf("Perimeter: %.2f\n", (*(circle.base.perimeter))(&circle));

    printf("\nSquare:\n");
    printf("Area: %.2f\n", (*(square.base.area))(&square));
    printf("Perimeter: %.2f\n", (*(square.base.perimeter))(&square));

    return 0;
}
