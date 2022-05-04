(define name "Brandon Horner")

(define (op? x)
  (cond ((and (symbol? x)(eq? x 'add)))
        ((and (symbol? x)(eq? x 'sub)))
        ((and (symbol? x)(eq? x 'mul)))
        (else #f)
        )
  )

(define (reserved? x)
  (cond ((and (symbol? x)(eq? x 'if)))
        ((and (symbol? x)(eq? x 'then)))
        ((and (symbol? x)(eq? x 'else)))
        ((and (symbol? x)(eq? x 'skip)))
        ((and (symbol? x)(eq? x 'while)))
        ((and (symbol? x)(eq? x 'do))) 
        ((and (symbol? x)(eq? x 'true)))
        ((and (symbol? x)(eq? x 'false)))
        (else #f)
        )
  )

(define (variable? x)
  (cond ((and
           (and (not(op? x)) (not(reserved? x)))
           (and (symbol? x) (char-alphabetic? (string-ref (symbol->string x) 0)))
           ))
        (else #f)
        )
  )

(define (constant? x)
  (cond ((and (not (char? x)) (not(string? x)) (not(variable? x))))
        (else #f)
        )
  )

(define (get_op x)
  (cond ((and (symbol? x) (eq? x 'add)) +)
        ((and (symbol? x) (eq? x 'sub)) -)
        ((and (symbol? x) (eq? x 'mul)) *)
    )
  )

(define number_args
  (lambda args
    (length args)))


(define (aexp? arg)
  (cond [(and (list? arg) (op? (list-ref arg 0))
              (or (constant? (list-ref arg 1)) (list? (list-ref arg 1)))
              (or (constant? (list-ref arg 2)) (list? (list-ref arg 2)))
              )]
        [(and (not(list? arg)) (or (constant? arg) (variable? arg) (char? arg)))]
  
        (else #f)
        )
  )

(define (bexp? args)
  (cond
    ;return true if symbol == 'true/'false 
    [(and (not(list? args)) (symbol? args) (or(eq? args 'true) (eq? args 'false)))]
    ;return true if list and list size = 2 AND first symbol in list == 'not
    [(and (list? args) (= (length args) 2) (symbol? (list-ref args 0)) (eq? (list-ref args 0) 'not)
         ; AND second symbol is 'true/'false OR list
          (or(bexp? (list-ref args 1)) (list? (list-ref args 1)))
          )]
    ;return true if list and list size = 3 AND first symbol in list == 'and
    [(and (list? args) (= (length args) 3) (symbol? (list-ref args 0)) (eq? (list-ref args 0) 'and)
          ;AND second symbol is 'true/'false OR list
          (or(bexp? (list-ref args 1)) (list? (list-ref args 1)))
          ;AND third symbol is 'true/'false OR list
          (or(bexp? (list-ref args 2)) (list? (list-ref args 2)))
          )]
    ;return true if list and list size = 3  
    [(and (list? args) (= (length args) 3)
          ;AND first symbol in list == 'eq OR 'lt
          (symbol? (list-ref args 0)) (or(eq? (list-ref args 0) 'eq) (eq? (list-ref args 0) 'lt))
          ;AND second symbol is 'true/'false OR list
          (or(bexp? (list-ref args 1)) (list? (list-ref args 1)))
          ;AND third symbol is 'true/'false OR list
          (or(bexp? (list-ref args 2)) (list? (list-ref args 2)))
          )]
        (else #f)   
    )
  )

;assumes a key and environment will be passed in
(define (find key env)
  (cond
    ((null? env) 0)
    (else
     (if (eqv? #f (assoc key env))
              0
              (cdr(assoc key env))
              )
          )     
     )
  )

;assumes you will enter an aexp and environment containing memory
(define (aexp e env)
  (cond
    [(bexp? e) display("error")]
    ;if e is a constant, return e
    [(or(integer? e)(constant? e)) e]
    
    ;if e is a variable, and if env is null return 0
    [(and(variable? e) (null? env)) 0]

    ;if e is a variable, and value of e in env is not null
    [(and(variable? e) (not(null? env))) (find e env)]
    
    ;if e is an aexp? see if second arg is list or variable
    [((aexp? e) (
                 (get_op(list-ref e 0)) 
                 (if
                  (list? (list-ref e 1))
                  ;if it is a list recur
                  (aexp (list-ref e 1) env)
                  ;else return 2nd elem of list
                  (list-ref e 1)) 
                 (if
                  (list? (list-ref e 2))
                  ;if it is a list recur
                  (aexp (list-ref e 2) env)
                  ;else return 3rd elem of list
                  (list-ref e 2))
                 ))]
    )
  )

(define (bexp e env)
  (cond
    [(and(integer? e) (null? env)) display("error")]
    [(and (eqv? e 'true) (null? env))]
    
    )
  )