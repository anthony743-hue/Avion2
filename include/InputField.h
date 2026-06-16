#pragma once
#include <QLineEdit>


using namespace std;

enum class InputType {
    Text,    // Aucune restriction (ou non vide)
    Number,  // Nombre flottant valide
    Date
};

class InputField : public QLin {
public :
    InputField(const string& label, InputType type);

    void setValue(const string& value);
    string getValue() const { return m_value; }
    float getFloat() const;
    void validate() const;

    InputType type() const { return m_type; }

private :
    string m_label;
    InputType m_type;
    string m_value;

    void validateNumber() const;
};