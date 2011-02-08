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
      'CamelCase': http://en.wikipedia.org/wiki/CamelCase
    - declarations of class members preferably indented to length longest type
    - class members names starting with 'm_', while arguments in constuctors
      staring with 'a_'
    - argument list in function whith a lot of arguments should be broken into
      multiple lines
    - all operators like plus, minus, equals should be surrounded by spaces
      preferably on both sides.
    - comments only to describe code briefly e.g. above a class or a function
    - preferably no comments inside functions
    - multiline comments surrounded by C comments, unless for nested comments

  useful reads:
    http://en.wikipedia.org/wiki/Indent_style
    https://computing.llnl.gov/linux/slurm/coding_style.pdf
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

// *** macros  and branching
#if (defined H_CHECK_SOMETHING) || (defined H_CHECK_SOMETHING_ELSE)
  #define H_IS_DEFINED
#else
  #define H_IS_NOT_DEFINED
#endif

// global method & swiwtch
int h_SomeGlobalMethod(int some_value)
{
  switch (some_value)
  {
    case 0:
      return some_value = 0;
    case 1:
      return some_value;
    default:
      return some_value;
  }
}

// *** struct
struct h_SomeStruct
{
  float         m_SomeMember1;
  unsigned int  m_SomeMember2;
};

// *** basic class
class h_SomeOtherObject
{
  public:
    h_SomeOtherObject(float a_SomeVar) {}
    ~h_SomeOtherObject() {}
};

// *** class with members
class h_SomeObject : h_SomeOtherObject
{
  private:
    float*        m_SomePtr;
    float         m_SomeVar;
    unsigned int  m_SomeItr;

    // *** one line in a function
    int member_function_1(void)
    {
      return 1;
    }
    int member_function_2(void)
    {
      return 2;
    }

  public:
    h_SomeObject(float a_SomeVar, float* a_SomePtr, unsigned int a_SomeItr)
    : h_SomeOtherObject(a_SomeVar)
    {
      // *** declarations on top
      float*        some_ptr, some_other_ptr;
      float         some_float;
      unsigned int  some_uint;

      // *** one line inside if, then, else statement
      if (a_SomePtr)
        member_function_1();
      else
        member_function_2();

      // *** multiple lines inside if, then, else statement
      if (a_SomeVar == 0)
      {
        member_function_1();
      }
      else
      {
        member_function_1();
        member_function_2();
      }

      // *** do, while & operators
      do
      {
        some_uint = ((2 * 4) + 1) & 0xff;
        some_float = 25.0 / 5.0;
      } while (0);
    }

    ~h_SomeObject() {}

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
};
