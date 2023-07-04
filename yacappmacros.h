#ifndef YACAPPMACROS_H
#define YACAPPMACROS_H

#include <cmath>

#define colorFromJSON(name, uppercasename) \
    set##uppercasename(config[#name].toString());

#define colorFromJSONIfExists(name, uppercasename) \
    if (o.contains(#name)) \
    { \
        set##uppercasename(o[#name].toString()); \
    }

#define stringFromJSON(name, uppercasename) \
    set##uppercasename(config[#name].toString());

#define doubleFromJSON(name, uppercasename) \
    set##uppercasename(config[#name].toDouble());

#define doubleFromJSONIfExists(name, uppercasename) \
    if (o.contains(#name)) \
    { \
        double value(o[#name].toDouble()); \
        if (value > 0) \
        { \
            set##uppercasename(value); \
        } \
    }

#define intFromJSON(name, uppercasename) \
    set##uppercasename(config[#name].toInt());

#define intFromJSONIfExists(name, uppercasename) \
    if (o.contains(#name)) \
    { \
        int value(o[#name].toInt()); \
        if (value > 0) \
        { \
            set##uppercasename(value); \
        } \
    }

#define boolFromJSON(name, uppercasename) \
    set##uppercasename(config[#name].toBool());

#define toJSON(name) \
    config[#name] = name()

#define intToJSON(name) \
    if (name()) \
    { \
        toJSON(name); \
    }

#define doubleToJSON(name) \
    if (std::fabs(name()) > 0.0001) \
    { \
        toJSON(name); \
    }

#define boolToJSON(name) \
    if (name()) \
    { \
        toJSON(name); \
    }

#define stringToJSON(name) \
    if (name().size()) \
    { \
        config[#name] = name(); \
    }

#define colorToJSON(colorName) \
    if (colorName().isValid() && colorName() != Qt::black) \
    { \
        config[#colorName] = colorName().name(); \
    }

#define stringlistToJSON(stringlistName) \
    if (stringlistName.size()) \
    { \
        QJsonArray array; \
        for (int i(0); i < stringlistName.size(); ++i) \
        { \
            array.append(stringlistName[i]); \
        } \
        config[#stringlistName] = array; \
    }

#define YACAPPCONSTANTPROPERTY(type, name, uppercasename, defaultvalue) \
    private: \
    type m_##name = {defaultvalue}; \
    public: \
    Q_PROPERTY(type name READ name) \
    type name() const {return m_##name;} \
    void set##uppercasename(type n){if (m_##name == n) return; m_##name = n;} \
    private:

#define YACAPP_READONLY_PROPERTY(type, name, defaultValue) \
    private: \
    type m_##name = {defaultValue}; \
    public: \
    Q_PROPERTY(type name READ name CONSTANT) \
    type name() const {return m_##name;} \
    private:

#define YACAPP_MODEL_DECLARATION(type, name) \
    private: \
    type m_##name; \
    public: \
    Q_INVOKABLE QAbstractItemModel *name() {return &m_##name;} \
    type &get##name() {return m_##name;} \
    private:

#define YACAPPPROPERTY(type, name, uppercasename, defaultvalue) \
    private: \
    type m_##name = {defaultvalue}; \
    public: \
    Q_PROPERTY(type name READ name WRITE set##uppercasename NOTIFY name##Changed) \
    type name() const {return m_##name;} \
    void set##uppercasename(type n){if (m_##name == n) return; m_##name = n; emit name##Changed();} \
    Q_SIGNAL \
    void name##Changed(); \
    private:

#define YACAPPPROPERTYCUSTOMSET(type, name, uppercasename, defaultvalue, setFunction) \
    private: \
    type m_##name = {defaultvalue}; \
    public: \
    Q_PROPERTY(type name READ name WRITE set##uppercasename NOTIFY name##Changed) \
    type name() const {return m_##name;} \
    setFunction \
    Q_SIGNAL \
    void name##Changed(); \
    private:

#define YACAPPPROPERTYWITHOPTIONS(type, name, uppercasename, defaultvalue, ...) \
    private: \
    type m_##name = {defaultvalue}; \
    QStringList m_##name##Options = {__VA_ARGS__}; \
    public: \
    Q_PROPERTY(type name READ name WRITE set##uppercasename NOTIFY name##Changed) \
    Q_PROPERTY(QStringList name##Options READ name##Options CONSTANT) \
    type name() const {return m_##name;} \
    QStringList name##Options() const {return m_##name##Options;} \
    void set##uppercasename(type n){if (m_##name == n) return; m_##name = n; emit name##Changed();} \
    Q_SIGNAL \
    void name##Changed(); \
    private:

#include <QtGlobal>
#include <QtGlobal>
#if QT_VERSION >= 0x060000
    #define mysizetype qsizetype
#else
#define mysizetype int
#endif

#define YACOBJECTLISTPROPERTY(type, singular, uppercasesingular, className) \
Q_PROPERTY(QQmlListProperty<type> singular##s READ singular##s CONSTANT) \
QList<type *> m_##singular##s; \
public: \
QQmlListProperty<type> singular##s() \
{ \
    return {this, this, \
                 &className::append##uppercasesingular, \
                 &className::singular##sCount, \
                 &className::singular, \
                 &className::clear##uppercasesingular##s, \
                 &className::replace##uppercasesingular, \
                 &className::removeLast##uppercasesingular}; \
} \
void append##uppercasesingular(type *singular) \
{ \
    m_##singular##s.append(singular); \
} \
mysizetype singular##sCount() const \
{ \
    return m_##singular##s.count(); \
} \
type *singular(mysizetype index) const \
{ \
    return m_##singular##s.at(index); \
} \
void clear##uppercasesingular##s() \
{ \
    for (mysizetype i(0); i < m_##singular##s.size(); ++i) \
    { \
        delete singular(i); \
    } \
    m_##singular##s.clear(); \
} \
void replace##uppercasesingular(mysizetype index, type *singular) \
{ \
    m_##singular##s[index] = singular; \
} \
void removeLast##uppercasesingular() \
{ \
    m_##singular##s.removeLast(); \
} \
private: \
static void append##uppercasesingular(QQmlListProperty<type> *list, type *singular) \
{ \
     reinterpret_cast< className *>(list->data)->append##uppercasesingular(singular); \
} \
static mysizetype singular##sCount(QQmlListProperty<type> *list) \
{ \
    return reinterpret_cast< className *>(list->data)->singular##sCount(); \
} \
static type* singular(QQmlListProperty<type> *list, mysizetype index) \
{ \
    return reinterpret_cast< className *>(list->data)->singular(index); \
} \
static void clear##uppercasesingular##s(QQmlListProperty<type> *list) \
{ \
    reinterpret_cast< className *>(list->data)->clear##uppercasesingular##s(); \
} \
static void replace##uppercasesingular(QQmlListProperty<type> *list, mysizetype index, type *singular) \
{ \
    reinterpret_cast< className *>(list->data)->replace##uppercasesingular(index, singular); \
} \
static void removeLast##uppercasesingular(QQmlListProperty<type> *list) \
{ \
    reinterpret_cast< className *>(list->data)->removeLast##uppercasesingular(); \
}

#endif // YACAPPMACROS_H
