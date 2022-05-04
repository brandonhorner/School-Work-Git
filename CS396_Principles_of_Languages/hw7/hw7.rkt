; Original Creator Of File - Frederic Loulergue
; Intended use: to complete homework 7
; Modified by Brandon Horner
(define name "Brandon Horner")

(define op?
  (lambda (op) 
    (and (symbol? op)
         (or (symbol=? op 'add)
             (symbol=? op 'sub)
             (symbol=? op 'mul)
             (symbol=? op 'div)))))

(define reserved?
  (lambda (keyword)
    (and (symbol? keyword)
         (or (symbol=? keyword 'true)
             (symbol=? keyword 'false)
             (symbol=? keyword 'if)
             (symbol=? keyword 'then)
             (symbol=? keyword 'else)
             (symbol=? keyword 'while)
             (symbol=? keyword 'do)))))

(define error?
  (lambda (arg . env)
    (if
     (and
      (symbol? arg)
             (or
              (symbol=? arg 'Undefined)
                 (symbol=? arg 'DivisionBy0)))
        #t
    (if
     (and   ;if it is a list of length three AND an aexp
      (list? arg) (= (length  arg) 3) (aexp? arg)
            ;see if div by 0 error
             (or
              (and
               (op? (car arg)) (eqv? (car arg) 'div) (= (caddr arg) 0))
              ;or if undefined error
              (or
               (eqv? (find (cadr arg) env) 'Undefined) (eqv? (find (caddr arg) env) 'Undefined))
               )
             )
     #t
     #f
      )
    )
    )
  )
             
(define variable?
  (lambda (arg)
    (and (symbol? arg)
         (not (reserved? arg))
         (char-alphabetic? (string-ref (symbol->string arg) 0)))))

(define constant?
  (lambda (n) (integer? n)))

(define aexp?
  (lambda (e)
    (or (variable? e)
        (constant? e)
        (and (list? e) (= (length  e) 3) (op? (car e)) (aexp? (cadr e)) (aexp? (caddr e))))))

  
(define bexp?
  (lambda (e)
    (or (and (symbol? e) (symbol=? e 'true))
        (and (symbol? e) (symbol=? e 'false))
        (and (list? e) (= (length e) 2) (symbol? (car e)) (symbol=? 'not (car e)) (bexp? (cadr e)))
        (and (list? e) (= (length e) 3) (symbol? (car e)) (symbol=? 'and (car e))
             (bexp? (cadr e)) (bexp? (caddr e)))
        (and (list? e) (= (length e) 3) (symbol? (car e)) (symbol=? 'eq (car e))
             (aexp? (cadr e)) (aexp? (caddr e)))
        (and (list? e) (= (length e) 3) (symbol? (car e)) (symbol=? 'lt (car e))
             (aexp? (cadr e)) (aexp? (caddr e))))))


(define find
  (lambda (var env)
    (if (null? env)
        'Undefined
        (if (symbol=? var (car (car env)))
            (cdr(car env))
            (find var (cdr env))
            )
        )
    )
  )

(define get_op
  (lambda (op)
    (cond
      ((symbol=? op 'add) +)
      ((symbol=? op 'sub) -)
      ((symbol=? op 'mul) *))))


(define aexp
  (lambda (e env)
    (cond
      ((constant? e) e)
      ((variable? e) (if (not (error? e)) (find e env) 'Undefined))
      ((error? e) 'Undefined)
      ((and (list? e) (aexp? e)) (if (error? e env) 'DivisionBy0))
      (else
       (let ((op (get_op (car e)))
                  (n1 (aexp (cadr e) env))
                  (n2 (aexp (caddr e) env)))
                  (op n1 n2)))
      )
    )
  )

(define bexp
  (lambda (e env)
    (cond
      ((and (symbol? e)(symbol=? e 'true)) #t)
      ((and (symbol? e)(symbol=? e 'false)) #f)
      ((=(length e) 2)
       (not (bexp (cadr e) env)))
      ((and (=(length e) 3)(symbol=? (car e) 'and))
       (and (bexp (cadr e) env) (bexp (caddr e) env)))
      ((and (=(length e) 3)(symbol=? (car e) 'eq))
       (= (aexp (cadr e) env) (aexp (caddr e) env))
       )
      (else (< (aexp (cadr e) env) (aexp (caddr e) env)))
      )
    )
  )