#include <sstream>
#include <iostream>
#include <string>
#include <fstream>

std::string svg_header = "<svg version=\"1.1\" baseProfile=\"full\" width=\"300\" height=\"200\" xmlns=\"http://www.w3.org/2000/svg\">\n";
std::string svg_footer = "</svg>";

using std::cout;
using std::endl;
using std::string;

int scale = 50;

class SingleQubitGate
{
    private:
        int _box_width = 40;
        int _box_height = 40;
        int _x = 0;
        int _y = 0;
        string _label;

    public:
        SingleQubitGate(string label)
        {
            _label = label;
        }

        string draw()
        {
            std::stringstream ss;
            ss << "<rect x=\""<< _x+5 << "\" y=\""<< _y+5 <<"\" width=\"40\" height=\"40\" stroke=\"black\" fill=\"white\" />\n";
            ss << "<line x1=\"" << _x << "\" y1=\"" << _y+25 << "\" x2=\"" << _x+5 << "\" y2=\"" << _y+25 << "\" stroke=\"black\" />" << "\n";
            ss << "<line x1=\"" << _x+45 << "\" y1=\"" << _y+25 << "\" x2=\"" << _x+50 << "\" y2=\"" << _y+25 << "\" stroke=\"black\" />" << "\n";
            ss << "<text font-family=\"" << "monospace" << "\" x=\"" << _x+25 << "\" y=\"" << _y+29 << "\" text-anchor=\"middle\">" << _label << "</text>";
            return ss.str();
        }

        void move_x(int spaces)
        {
            _x += scale*spaces;
        }
        void move_y(int spaces)
        {
            _y += scale*spaces;
        }

};



class Wire
{
    private:
        int _x=0, _y=0;
        string _colour = "black";

    public:
        string draw()
        {
            std::stringstream ss;
            ss << "<line x1=\"" << _x << "\" y1=\"" << _y+25 << "\" x2=\"" << _x+50 << "\" y2=\"" << _y+25 << "\" stroke=\"" << _colour << "\" />" << "\n";
            return ss.str();
        }
        void move_x(int spaces)
        {
            _x += scale*spaces;
        }
        void move_y(int spaces)
        {
            _y += scale*spaces;
        }
};

int main()
{
    std::ofstream file("output.svg", std::ofstream::out);
    
    SingleQubitGate gate1{"A"};
    SingleQubitGate gate2{"B"};
    SingleQubitGate gate3{"H"};
    Wire wire1;
    Wire wire2;
    
    gate1.move_x(1);
    wire2.move_x(2);
    gate3.move_x(1);
    gate3.move_y(1);
    gate2.move_x(3);
    gate2.move_y(1);
    file << svg_header << wire1.draw() << gate1.draw() <<  wire2.draw() << gate2.draw() << gate3.draw() << svg_footer;
    file.close();
    return 1;
}
