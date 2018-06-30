## Comments
#### Program header atop every file
```
/*******************************************************************************
** Program Filename:
** Author:
** Date:
** Description:
** Input:
** Output:
*******************************************************************************/
```

#### Function header atop every function
```
/*******************************************************************************
** Function:
** Description: purpose, usage goes here
** Parameters: type of any parameters goes here
** Pre-Conditions: conditions that must hold prior to calling the function
** Post-Conditions: conditions that hold upon exiting the function
*******************************************************************************/
```

#### Comment algorithms and data structures in block form
```
/*
** Here is a block comment.
** The comment text should be tabbed or spaced over uniformly.
** The opening slash-star and closing star-slash are alone on a line.
*/
```

#### Comments inside functions
##### Own line
```
int main() {
    float radius;
    /* Read radius value from user. */
    /* match the indentation of the line you are commenting */
    /* and put the comment above the line */
    printf(“Enter the circle’s radius value: ”);
    scanf(“%f”, &radius);
    return 0;
}
```

##### In-line
```
Very short comments may appear on the same line as the code they describe
Align them off to the right
if (a = = EXCEPTION) {
    b = TRUE;               /* special case */
}
else {
    b = isprime(a);         /* works only for odd a */
}
```

## Whitespace
- 2 blank lines between functions
- indentation with 4 spaces
- spaces after commas, semicolons, operators and variables

## Variable Declarations
- Related declarations of the same type go on the same line
- Unrelated declarations get their own line
- Comment on role of object declared
- Don't comment constants made in `#define` (name should be self-documenting)
- The "pointer" qualifier, '*', should be with the variable name: `char *s, *t, *u;`

## Compound Statements
- List of statements enclosed by braces to form a block
#### Opening brace (`{`) on line of control and closing brace (`}`) on own line
```
control {
   statement;
   statement;
}
```

#### if-else
```
if (expr) {
    statement;
} else {
    statement;
    statement;
}
```
#### if-if-else
```
if (ex1) {
    if (ex2) {
        funca();
    }
} else {
    funcb();
}
```
#### if-else with else if
```
if (STREQ (reply, "yes")) {
    statements for yes
    ...
} else if (STREQ (reply, "no")) {
    ...
} else if (STREQ (reply, "maybe")) {
    ...
} else {
    statements for default
    ...
}
```
#### block with multiple labels
- labels get their own lines
- comment where you omit `break` statement for fallthrough
- if you don't end with a `default` case then add a break to the last statement to prevent implicit fallthrough if another statement is added
```
switch (expr) {
    case ABC:
    case DEF:
        statement;
        break;
    case UVW:
        statement; /*FALLTHROUGH*/
    case XYZ:
        statement; break;
}
```

## Naming Conventions
- use `snake_case`
- don't use leading or trailing underscores `_dont_do_this  or_this_  _or_this_`
- `#define` constants ALL_CAPS
- Enum constants are in all CAPS.


## Constants
#### Name numerical and other symbolic constants using `#define` eg ```MEANING = 42```
- Constants should be defined consistently with their use: e.g. use 540.0 for a float, not 540 with an implicit float cast.
- Always compare pointers to NULL, rather than 0.
#### Use enumeration data type for sets of values
##### BAD
```
#define UNKNOWN  0
#define SUNDAY   1
#define MONDAY   2
#define TUESDAY  3
...
#define SATURDAY 7

// later used like this
void calendar_set_weekday(int wd);

```
##### GOOD
```
typedef enum {
    UNKNOWN
,   SUNDAY
,   MONDAY
,   TUESDAY
,   ...
,   SATURDAY
} Weekday;

// later used like this
void calendar_set_weekday(Weekday wd);

```

#### Booleans
- lots of debate, but just use 1/0 or import the std lib version available in c99
