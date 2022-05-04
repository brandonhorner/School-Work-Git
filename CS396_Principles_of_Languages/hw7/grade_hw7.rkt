;============ UTILITY FUNCTIONS ===========

(define (sum l) (foldl + 0 l))

(define (compare p1 p2)
  (cond ((and (symbol? p1) (symbol? p2) (eq? p1 p2)))
        ((and (boolean? p1) (boolean? p2) (eqv? p1 p2)))
        ((and (number? p1) (number? p2) (= p1 p2)))
        ((and (string? p1) (string? p2) (string=? p1 p2)))
        ((and (list? p1) (list? p2) (= (length p1) (length p2)))
         (foldl (lambda (x y)(and x y)) #t (map compare p1 p2)))
        (else #f))
  )
; print-errors: exn -> void
; (print-errors exn) print a message
; corresponding to the exception exn 
(define print-errors
  (lambda (exn)
    (begin
      (display "FAILED (")
      (display (exn-message exn))
      (display ")")
      (newline)
      #f
      )))

; header: string symbol any boolean -> void
; (header name test-expr result mode)
; prints the header of test messages
; - name is the title of the test
; - test-expr is the quoted expression to test
; - result is the expected result
; - mode is the mode in which to print the result

(define (header name test-expr result err-mode)
  (begin
    (display "== Test ")(display name)(display "  ==")
    (newline)
    (display "Tested expression: ")(write test-expr)
    (newline)
    (display "Expected result:   ")
    ((if err-mode display write)result)
    (newline)
    (display "Test: ")
    ))
; test-error: string symbol -> void
; (test-error name test-expr)
; evaluates the quoted expression test-expr
; that is supposed to fail
(define (test-error header name test-expr)
  (with-handlers ((exn:fail? (lambda (exn) (begin (displayln "Test: PASSED")#t))))
    (begin (let ((test-result (eval test-expr)))
             (begin
               (header name test-expr "the execution should raise an error" #f)
               (display "FAILED (result is ")
               (display test-result)
               (displayln ")")
               #f)))))
; test: string symbol any predicate -> void
; (test-error name test-expr result pred?)
; evaluates the quoted expression test-expr
; that is supposed evaluate to result.
; The values are compared using the predicate pred?
(define (test header name test-expr result equiv?)
  (with-handlers ((exn:fail? print-errors))
    (let ((test-result (eval test-expr)))
      (if (equiv? test-result result)
          (begin (displayln "Test: PASSED") #t)
          (begin (header name test-expr result #f)
                 (display "FAILED (unexpected result: ")
                 (display test-result)
                 (displayln ")")
                 #f))))
  )

;; --------- The tests -------------

(define (grade-header name test-expr result err-mode)
  (begin
    (displayln "Test: ")
    (display "\tTested expression: ")(write test-expr)
    (display "\n")
    (display "\tExpected result:   ")
    ((if err-mode display write)result)
    (display "\n\t")
    ))

(define (tests name points test-list)
  (begin
    (display "PROCEDURE: ")(displayln name)
    (define score 
      (sum (map (lambda (t) (if t points 0))
                (map (lambda (l)(if (= (length l) 4)
                                    (apply test grade-header l)
                                    (apply test-error grade-header l)))
                       test-list))))
    (display "Points: ")(display score)
    (newline)
    score
    )
  )

(define (final_grade scores)
  (begin
    (newline)
    (display "GRADE: ")
    (displayln (sum scores)))
  )
;==========================================

(load "hw7.rkt")

(begin
  (displayln "---------------------------------")
  (displayln "ASSIGNMENT 7")
  (displayln "---------------------------------")
  (with-handlers
      ((exn:fail? (lambda (exn)
                    (begin
                      (displayln "Missing definition 'name'")
                      (final_grade null)
                      ))))
    (display "Student: ")
    (displayln name)
    (displayln "---------------------------------")
    (newline)
    (final_grade
     (list
      (tests "error?" 0.25
             (list
              (list "" '(error? 'div) #f compare)
              (list "" '(error? (list 'add 1 'x)) #f compare)
              (list "" '(error? 'Undefined) #t compare)
              (list "" '(error? 'DivisionBy0) #t compare)
              )
             )
      (tests "find" 0.25
             (list
              (list "" '(find 'x null) 'Undefined compare)
              (list "" '(find 'y (list (cons 'x 42) (cons 'y 1))) 1 compare)
              (list "" '(find 'z (list (cons 'x 42) (cons 'y 10))) 'Undefined compare)
              (list "" '(find 'x (list (cons 'y 10) (cons 'x 3)(cons 'x 2)(cons 'x 2)(cons 'x 2)(cons 'x 2)(cons 'x 2)(cons 'x 2))) 3 compare)
              )
             )
      (tests "aexp" 0.25
             (list
              (list "" '(aexp 'true null))
              (list "" '(aexp 42 null) 42 compare)
              (list "" '(aexp 'x null) 'Undefined compare)
              (list "" '(aexp (list 'add 'x (list 'div 'y 'z)) (list (cons 'x 1)(cons 'x 2)(cons 'y 2))) 'Undefined compare)
              (list "" '(aexp (list 'add 'x (list 'div 'y 'z)) (list (cons 'x 1)(cons 'y 2))) 'Undefined compare)
              (list "" '(aexp (list 'add 'x (list 'div 'y 'z)) (list (cons 'x 1)(cons 'y 2)(cons 'z 0))) 'DivisionBy0 compare)

              )
             )
      (tests "bexp" 0.25
             (list
              (list "" '(bexp 42 null))
              (list "" '(bexp 'true null) #t compare)
              (list "" '(bexp (list 'lt 'x (list 'div 'y 'z)) (list (cons 'x 1)(cons 'x 2)(cons 'y 2))) 'Undefined compare)
              (list "" '(bexp (list 'eq 'x (list 'div 'y 'z)) (list (cons 'x 1)(cons 'y 2))) 'Undefined compare)
              (list "" '(bexp (list 'lt 'x (list 'div 'y 'z)) (list (cons 'x 1)(cons 'y 2)(cons 'z 0))) 'DivisionBy0 compare)
              (list "" '(bexp (list 'eq 'x (list 'div 'y 'z)) (list (cons 'x 0)(cons 'y 2)(cons 'z 3))) #t compare)
              )
             )
      )
     )
    )
  )