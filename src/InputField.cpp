#include "InputField.h"
#include <iostream>
#include <charconv>
#include <system_error>

using namespace std;

InputField::InputField(const string& label, InputType type) :
    m_label(label), m_type(type)
{}

void InputField::setValue(const string& value){
    m_value = value;
}

void InputField::validate() const {
    switch (m_type){
        case InputType::Number:
            validateNumber();
            break;
        default:
            break;
    }
}

void InputField::validateNumber() const {
    float f = 0.f;
    auto [ptr, ec] = from_chars(m_value.data(), m_value.data() + m_value.size(),f);
    if( ec != errc{} || ptr != m_value.data() + m_value.size() )
        throw invalid_argument("valeur numérique invalide (" + m_value + ")");
}

float InputField::getFloat() const { 
    return stof(m_value);
}

