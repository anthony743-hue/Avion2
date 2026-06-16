#include "InputField.h"
#include <charconv>
#include <stdexcept>
#include <system_error>

using namespace std;

InputField::InputField(const string &label, InputType type, QWidget *parent) :
    QLineEdit(parent), m_label(label), m_type(type)
{
    setPlaceholderText(QString::fromStdString(label));
}

void InputField::setValue(const string &value) {
    setText(QString::fromStdString(value));
}

string InputField::getValue() const {
    return text().toStdString();
}

void InputField::validate() const {
    switch (m_type) {
        case InputType::Number:
            validateNumber();
            break;
        default:
            if (text().trimmed().isEmpty())
                throw invalid_argument("Le champ \"" + m_label + "\" ne doit pas être vide.");
            break;
    }
}

void InputField::validateNumber() const {
    const string value = getValue();
    float f = 0.f;
    auto [ptr, ec] = from_chars(value.data(), value.data() + value.size(), f);
    if (ec != errc{} || ptr != value.data() + value.size())
        throw invalid_argument("Valeur numérique invalide pour \"" + m_label + "\" (" + value + ")");
}

float InputField::getFloat() const {
    return stof(getValue());
}
