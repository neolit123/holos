/*
  holos library coding style (HLCS)
  (just a proposal at this stage)

  last updated:
    0802112

  guide:
    - 2 space indentation (not real tabs)
    - code should fit 80 columns limit
    - no whitespace at end of lines
    - all macros definitions preferably before other code
    - all caps names for non-function macro definitions (e.g. H_SOMEDEF)
    - '\' at the same end column of macros with multiple lines
    - in functions and classes, declarations of variables should be
      before code
    - braces bellow function, if/then/else statement or class definition
    - no braces needed for one line of code in if/then/elese statements
    - braces are needed for more than one line of code in if/then/else
      statements
    - function that contain only one line of code should have the braces
      on separate lines surrounding the code
    - global objects and function names should start with 'h_' and use
      'CamelCase'.
    - declarations of class members preferably indented to length longest type
    - class members names starting with 'm_', while arguments in constuctors
      staring with 'a_'
    - argument list in function whith a lot of arguments should be broken into
      multiple lines
    - all operators like plus, minus, equals should be surrounded by spaces
      preferably.
    - comments only to describe code briefly e.g. above a class or a function
    - preferably no comments inside functions
    - multiline comments surrounded by C comments, unless for nested comments
    - commenting out large code block should be done with "#if 0" or
      more descriptively "#if NOT_ENABLED"
    - unused variables should be solved or marked with __attribute__((unused))
      where the attribute is placed after the variable type

  examples of coding styles:
    http://en.wikipedia.org/wiki/Indent_style
    https://computing.llnl.gov/linux/slurm/coding_style.pdf
    http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml
    http://geosoft.no/development/cppstyle.html
*/

/*
  ***
  a
  comment
  on
  multiple
  lines
*/

// *** function-like macro on multiple lines
#define h_SomeMacro(x, y, z)                  \
  do_something_with_macro(x, y, z);           \
  do_something_else_with_macro(x, y, z)

// *** other macro definitions with uppercase   
#define _UNUSED __attribute__((unused))

// *** macros and branching
#if (defined H_CHECK_SOMETHING) ||            \
    (defined H_CHECK_SOMETHING_ELSE_1) ||     \
    (defined H_CHECK_SOMETHING_ELSE_2)
  #define H_IS_DEFINED
#else
  #define H_IS_NOT_DEFINED
#endif

// *** global method; switch statement
int h_SomeGlobalMethod(int some_value)
{
  switch (some_value)
  {
    case 0:
      some_value = 0;
      return some_value;
    case 1:
      // *** no need to put a less complicated return in braces
      return some_value + 1;
    default:
      return some_value + 2;
  }
}

// *** struct; declarations are aligned
struct h_SomeStruct
{
  float         m_SomeMember1;
  unsigned int  m_SomeMember2;
};

// *** enums should use upper case names with "_" separator
enum h_SomeEnum
{
  VAL_ONE,
  VAL_TWO,
  VAL_THREE
};

// *** basic class
class h_SomeOtherObject
{
  public:
    // *** empty methods can be on single line
    h_SomeOtherObject(float _UNUSED a_SomeVar) {}
    ~h_SomeOtherObject() {}
};

// *** derived class with members
class h_SomeObject : h_SomeOtherObject
{
  private:
    float*        m_SomePtr;
    float         m_SomeVar;
    unsigned int  m_SomeItr;

    // *** short functions are still defined on a new line
    int member_function_1(void)
    {
      return 1;
    }
    int member_function_2(void)
    {
      return 2;
    }

  public:
    h_SomeObject( float a_SomeVar,
                  float* a_SomePtr,
                  unsigned int _UNUSED a_SomeItr )
      : h_SomeOtherObject(a_SomeVar)
    {
      // *** variables in declaration blocks are aligned
      float        *some_ptr, *some_other_ptr;
      float         some_float;
      unsigned int  some_uint;
      
      some_ptr = &some_float;
      some_other_ptr = &some_float;
      
      // *** one line inside "if, then, else" statement
      if (a_SomePtr)
        member_function_1();
      else
        member_function_2();

      // *** multiple lines inside "if, then, else" statement
      if (a_SomeVar == 0.f)
      {
        member_function_1();
      }
      else
      {
        member_function_1();
        member_function_2();
      }

      // *** "do, while" statement; operators with spaces
      do
      {
        some_uint = (2 + 5) >> 1;
        some_float = 25.f / 2.f;
      } while (0);
    }

    ~h_SomeObject() {}

    // *** disabled section is disabled with a macro and not commented out
    #if IS_NOT_ENABLED

    // *** function with a lot of arguments
    void function_with_many_arguments(float a_SomeVar1,
                                      float* a_SomePtr1,
                                      unsigned int a_SomeItr1,
                                      float a_SomeVar2,
                                      float* a_SomePtr2,
                                      unsigned int a_SomeItr2)
    {
      a_SomeVar1 += a_SomeVar2;
      a_SomeItr1 += a_SomeItr2;
    }

    #endif // #if IS_NOT_ENABLED

};
