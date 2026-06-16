#pragma once
#include <QLineEdit>
#include <string>

enum class InputType {
    Text,    // Aucune restriction (ou non vide)
    Number,  // Nombre flottant valide
    Date
};

// Champ de saisie : un QLineEdit enrichi d'un libellé et d'une règle de
// validation (texte / nombre / date).
class InputField : public QLineEdit {
public:
    explicit InputField(const std::string &label, InputType type, QWidget *parent = nullptr);

    void setValue(const std::string &value);
    std::string getValue() const;
    float getFloat() const;
    void validate() const;

    const std::string &label() const { return m_label; }
    InputType type() const { return m_type; }

private:
    std::string m_label;
    InputType m_type;

    void validateNumber() const;
};
