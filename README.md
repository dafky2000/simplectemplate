# simplectemplate
Master: [![Build Status](https://travis-ci.org/dafky2000/simplectemplate.svg?branch=master)](https://travis-ci.org/dafky2000/simplectemplate) [![Coverage Status](https://coveralls.io/repos/github/dafky2000/simplectemplate/badge.svg?branch=master)](https://coveralls.io/github/dafky2000/simplectemplate)

Develop: [![Build Status](https://travis-ci.org/dafky2000/simplectemplate.svg?branch=develop)](https://travis-ci.org/dafky2000/simplectemplate) [![Coverage Status](https://coveralls.io/repos/github/dafky2000/simplectemplate/badge.svg?branch=develop)](https://coveralls.io/github/dafky2000/simplectemplate)

Simple C Template library to expand placeholders in a template string, simple alternative to mustache.github.io that doesn't require JSON as data input. The main focus of this project is accessibility, making it easy and fast to hack into any project. Aiming to support native mustache format.

## Building and running examples / tests
```sh
make clean
make
make test
```

## How to include simplectemplate
* Add template_functions.c and template_function.h to your project and build process.
* Add ```#include "template_functions.h"``` in your program.
* Start coding, easy!

## Use case
Say you want to generate an HTML file from the template below in C:

```html
<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>{{title}}</title>
  </head>
  <body>
    <h1 class="{{is_error error_class}}">{{title}}</h1>
    {{body}}
    {{#is_not_error}}
    <p>In addition to the is_error above with space separating the data, simplectemplate also supports opening and closing braces like this! One step closer to supporting mustache formatted templates!</p>
    <p>In the short term future, this will be the same sytax for looped data</p>
    {{/is_not_error}}
  </body>
</html>
```

While libraries like https://gitlab.com/jobol/mustach/ exist for this type of task, there are simply not simple enough! The Mustache C implementation requires than you submit the data via JSON which opens up another can of worms in C. Not natively supported in C and the available libraries are not easy to use at all. With `simplectemplate` you submit your data as string arrays.

Example code:

```c
const char *data[] = {
  "title", "My super cool website",
  "body", "Put whatever you want in the body! Heck, even another rendered template ;)"
  "is_error", "true or anything other than NULL or empty string",
  "is_not_error", NULL, // "Evaluates" to false
  "is_not_error", "", // Also "evaluates" to false
};

// Render the template and replace the template variables
char *template = render_template("<!DOCTYPE html> \n\
<html> \n\
  <head> \n\
    <meta charset="UTF-8"> \n\
    <meta name="viewport" content="width=device-width, initial-scale=1" /> \n\
    <title>{{title}}</title> \n\
  </head> \n\
  <body> \n\
    <h1 class="{{is_error error_class}}">{{title}}</h1> \n\
    {{body}} \n\
    {{#is_not_error}} \n\
    <p>In addition to the is_error above with space separating the data, simplectemplate also supports opening and closing braces like this! One step closer to supporting mustache formatted templates!</p> \n\
    <p>In the short term future, this will be the same sytax for looped data</p> \n\
    {{/is_not_error}} \n\
  </body> \n\
</html>", 2, data);

printf("%s\n", template);
```

Renders to:

```html
<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>My super cool website</title>
  </head>
  <body>
    <h1 class="error_class">My super cool website</h1>
    Put whatever you want in the body! Heck, even another rendered template ;)

  </body>
</html>
```

# RenderOptions - Custom opening/closing/condition prefix, condition separator, and end-block prefixes...

```c
const char *data[] = {
  "title", "My super cool website",
  "body", "Put whatever you want in the body! Heck, even another rendered template ;)"
  "is_error", "true or anything other than NULL or empty string",
  "is_not_error", NULL, // "Evaluates" to false
  "is_not_error", "", // Also "evaluates" to false
};

// Render the template and replace the template variables
char *template = my_render_template("<!DOCTYPE html> \n\
<html> \n\
  <head> \n\
    <meta charset="UTF-8"> \n\
    <meta name="viewport" content="width=device-width, initial-scale=1" /> \n\
    <title>$[[data.title]]</title> \n\
  </head> \n\
  <body> \n\
    <h1 class="$[[is_error error_class]]">$[[title]]</h1> \n\
    $[[body]] \n\
    $[[?is_not_error]] \n\
    <p>In addition to the is_error above with space separating the data, simplectemplate also supports opening and closing braces like this! One step closer to supporting mustache formatted templates!</p> \n\
    <p>In the short term future, this will be the same sytax for looped data</p> \n\
    $[[-is_not_error]] \n\
  </body> \n\
</html>", 2, data, (struct RenderOptions){.placeholder_open="$[[", .placeholder_close="]]", .data_cond_open_prefix="?", .data_cond_close_prefix="-", .data_cond_separator=" "});

printf("%s\n", template);
```

# Helper functions

## Render from file

```c
const char *data[] = {
  "title", "My super cool website",
  "body", "Put whatever you want in the body! Heck, even another rendered template ;)"
};

// Render the template and replace the template variables
char *template = render_template_file("./templates/index.html", 2, data);
printf("%s\n", template);
```

Outputs:

```html
<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>My super cool website</title>
  </head>
  <body>
    <h1>My super cool website</h1>
    Put whatever you want in the body! Heck, even another rendered template ;)
  </body>
</html>
```
