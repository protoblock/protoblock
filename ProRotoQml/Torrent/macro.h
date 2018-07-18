#ifndef MACRO
#define MACRO

#define SETTER(varType, varName) void Set##varName( const varType &newValue ){ \
        m_##varName = newValue; \
    }

#define SETTER_SIMPLE(varType, varName) void Set##varName( const varType &newValue ){ \
        m_##varName = newValue; \
        emit varName##Changed( newValue ); \
    }

#define SETTER_ABOUT(varType, varName) void Set##varName( const varType &newValue ){ \
        emit varName##AboutToBeChanged( m_##varName, newValue ); \
        m_##varName = newValue; \
        emit varName##Changed( newValue ); \
    }

#define GETTER(varType, varName) const varType &Get##varName() const { return m_##varName; }

#define PROP_GET(varType, varName) private : varType m_##varName; \
    public : GETTER(varType, varName)

#define PROP_ALL(varType, varName) PROP_GET(varType, varName) \
    public slots: SETTER_ABOUT(varType, varName)\

#define PROP_SIMPLE(varType, varName) PROP_GET(varType, varName) \
    public slots: SETTER_SIMPLE(varType, varName)

#define PROP_GET_SET(varType, varName) PROP_GET(varType, varName) \
    public slots: SETTER(varType, varName)

#define PROP_SIMPLE_(varType, varName) PROP_GET(varType, varName) \
    public slots: SETTER_SIMPLE(varType, varName) \
    signals: void varName##Changed( const varType &newVal );

#endif // MACRO
