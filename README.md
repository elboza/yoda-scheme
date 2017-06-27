## Yoda-Scheme

yet another scheme interpreter

### build

```
make help     #(to see make options)
make
sudo make install

```

#### run

```
Yoda-Scheme v0.1

USAGE: yscm [options] [file]
valid options:
-i              --shell         interactive (shell mode)
-h              --help          show this help
-v              --version       prints scheme version number
-s              --stdin         read prog from stdin
-f file         --file          read file
-x 'prog'       --exec          read & execute prog


```

#### Done

```

quote
 define
 set!
 if
 lambda
 begin
 cond
 else
 let
 and
 or
 +
 -
 *
 =
 <
 >
 remainder
 quotient
 eval
 apply
 map
 reverse
 lenght
 for-each
 not
 append
 car
 cdr
 ca*d*r
 null?
 pair?
 number?
 integer?
 boolean?
 float?
 string?
 char?
 char->integer
 integer->char
 symbol->string
 number->string
 string->symbol
 open-input-port
 open-output-port
 close-input-port
 close-output-port
 write
 read
 load
 write-char
 cons
 list
 eq?
 read-char
 peek-char
 output-port?
 function &rest params ( . arg)
 let*
 letrec
 defmacro
 display
 ...

```


#### TODO

```

 define-syntax
 let-syntax
 syntax-rules
 delay
 force
 zero?
 vector
 make-string
 string-ref
 vector-ref
 string->list
 list->string
 vector->list
 list->vector

```

