#ifndef YACAPPMACROS_H
#define YACAPPMACROS_H

#define stringFromJSON(name, uppercasename) \
    set##uppercasename(config[#name].toString());

#define doubleFromJSON(name, uppercasename) \
    set##uppercasename(config[#name].toDouble());

#define toJSON(name) \
    config[#name] = name()

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

#define YACAPPCONSTANTPROPERTY(type, name, uppercasename, defaultvalue) \
    private: \
    type m_##name = {defaultvalue}; \
    public: \
    Q_PROPERTY(type name READ name) \
    type name() const {return m_##name;} \
    void set##uppercasename(type n){if (m_##name == n) return; m_##name = n;} \
    private:

#define YACAPPREADONLYPROPERTY(type, name, defaultvalue) \
    private: \
    type m_##name = {defaultvalue}; \
    public: \
    Q_PROPERTY(type name READ name NOTIFY name##Changed) \
    type name() const {return m_##name;} \
    Q_SIGNAL \
    void name##Changed(); \
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
int singular##sCount() const \
{ \
    return m_##singular##s.count(); \
} \
type *singular(int index) const \
{ \
    return m_##singular##s.at(index); \
} \
void clear##uppercasesingular##s() \
{ \
    for (int i(0); i < m_##singular##s.size(); ++i) \
    { \
        delete singular(i); \
    } \
    m_##singular##s.clear(); \
} \
void replace##uppercasesingular(int index, type *singular) \
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
static int singular##sCount(QQmlListProperty<type> *list) \
{ \
    return reinterpret_cast< className *>(list->data)->singular##sCount(); \
} \
static type* singular(QQmlListProperty<type> *list, int index) \
{ \
    return reinterpret_cast< className *>(list->data)->singular(index); \
} \
static void clear##uppercasesingular##s(QQmlListProperty<type> *list) \
{ \
    reinterpret_cast< className *>(list->data)->clear##uppercasesingular##s(); \
} \
static void replace##uppercasesingular(QQmlListProperty<type> *list, int index, type *singular) \
{ \
    reinterpret_cast< className *>(list->data)->replace##uppercasesingular(index, singular); \
} \
static void removeLast##uppercasesingular(QQmlListProperty<type> *list) \
{ \
    reinterpret_cast< className *>(list->data)->removeLast##uppercasesingular(); \
}

#endif // YACAPPMACROS_H
