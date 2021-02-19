#include <sstream>
#include <iostream>
#include <string>
#include <fstream>

std::string svg_header = "<svg version=\"1.1\" baseProfile=\"full\" width=\"600\" height=\"600\" xmlns=\"http://www.w3.org/2000/svg\">\n";
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
            ss << "<text font-family=\"" << "monospace" << "\" x=\"" << _x+25 << "\" y=\"" << _y+29 << "\" text-anchor=\"middle\">" << _label << "</text>";
            return ss.str();
        }

        void move_x(int spaces)
        {
            _x = scale*spaces;
        }
        void move_y(int spaces)
        {
            _y = scale*spaces;
        }
        void move(int x, int y)
        {
            _x = scale*x;
            _y = scale*y;
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
            _x = scale*spaces;
        }
        void move_y(int spaces)
        {
            _y = scale*spaces;
        }
        void move(int x, int y)
        {
            _x = scale*x;
            _y = scale*y;
        }
};

class Label
{
    private:
        int _box_width = 40;
        int _box_height = 40;
        int _x = 0;
        int _y = 0;
        string _label;

    public:
        Label(string label)
        {
            _label = label;
        }

        string draw()
        {
            std::stringstream ss;
            ss << "<text font-family=\"" << "monospace" << "\" x=\"" << _x+25 << "\" y=\"" << _y+29 << "\" text-anchor=\"middle\">" << _label << "</text>";
            return ss.str();
        }

        void move_x(int spaces)
        {
            _x = scale*spaces;
        }
        void move_y(int spaces)
        {
            _y = scale*spaces;
        }
        void move(int x, int y)
        {
            _x = scale*x;
            _y = scale*y;
        }
};

class Control
{
    private:
        int _x = 0;
        int _y = 0;
        int _size;
        string _label;
        string _colour = "black";

    public:
        Control(string label, int size)
        {
            _size = size;
            _label = label;
        }

        string draw()
        {
            std::stringstream ss;
            ss << "<line x1=\"" << _x+25 << "\" y1=\"" << _y+25 << "\" x2=\"" << _x+25 << "\" y2=\"" << _y+25+_size*scale << "\" stroke=\"" << _colour << "\" />" << "\n";
            ss << "<rect x=\""<< _x+5 << "\" y=\""<< _y+5+scale*_size <<"\" width=\"40\" height=\"40\" stroke=\"black\" fill=\"white\" />\n";
            ss << "<circle cx=\""<< _x+25 <<"\" cy=\""<< _y+25 <<"\" r=\"3\" />";
            return ss.str();
        }
        
        void move_x(int spaces)
        {
            _x = scale*spaces;
        }
        void move_y(int spaces)
        {
            _y = scale*spaces;
        }
        void move(int x, int y)
        {
            _x = scale*x;
            _y = scale*y;
        }
};

class MultiQubitGate
{
    private:
        int _box_width = 40;
        int _box_height = 40;
        int _size;
        int _x = 0;
        int _y = 0;
        string _label;

    public:
        MultiQubitGate(string label, int size)
        {
            _label = label;
            _size = size;
        }

        string draw()
        {
            std::stringstream ss;
            ss << "<rect x=\""<< _x+5 << "\" y=\""<< _y+5 <<"\" width=\"40\" height=\"" << _size*scale-10 << "\" stroke=\"black\" fill=\"white\" />\n";
            ss << "<text font-family=\"" << "monospace" << "\" x=\"" << _x+25 << "\" y=\"" << _y+29 << "\" text-anchor=\"middle\">" << _label << "</text>";
            return ss.str();
        }

        void move_x(int spaces)
        {
            _x = scale*spaces;
        }
        void move_y(int spaces)
        {
            _y = scale*spaces;
        }
        void move(int x, int y)
        {
            _x = scale*x;
            _y = scale*y;
        }
};

int main()
{
    std::ofstream file("output.svg", std::ofstream::out);
    file << svg_header;

    Wire wire;
    SingleQubitGate gate1{"A"};
    SingleQubitGate gate2{"B"};
    SingleQubitGate gate3{"H"};
    MultiQubitGate mgate1{"M", 2};
    Label label1{"\u007c0\u3009"};
    Control control1{"X", -2};

    for (int i = 0+1; i < 5+1; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            wire.move(i, j);
            file << wire.draw();
        }
    }

    for (int i = 0; i < 5; i++)
    {
        label1.move(0, i);
        file << label1.draw();
    }

    control1.move(5,2);
    file << control1.draw();

    mgate1.move(4,1);
    file << mgate1.draw();

    gate1.move(1,1);
    file << gate1.draw();

    gate3.move(3,2);
    file << gate3.draw();

    file << svg_footer;
    file.close();
    return 1;
}
