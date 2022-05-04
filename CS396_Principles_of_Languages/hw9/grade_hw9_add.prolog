:- initialization main.

sum([], 0).
sum([H|T], Sum) :- number(H), sum(T, S), Sum is S+H.

main(_) :-
    write('== Assignment 9 ==\n'),
    consult('hw9.prolog'),
    (catch(name(Name), E, (print_message(error, E),fail)) ->
	 (write('Student: '),write(Name),write('\n'),
    % Test nat
	  write('\nTEST nat\n'),
    (catch(nat(o), E, (print_message(error, E),fail)) ->
	 (write('nat(o) succeeds: 1 point\n'), L1=1);
     (write('nat(o) fails: 0 point\n'), L1=0)),
    (catch(\+nat(toto), E, (print_message(error, E),fail)) ->
	 (write('\\+nat(toto) succeeds: 1 points\n'), L2=1);
     (write('\\+nat(toto) fails: 0 point\n'), L2=0)),
    (catch(nat(s(s(o))), E, (print_message(error, E),fail)) ->
	 (write('nat(s(s(o))) succeeds: 1 points\n'), L3=2);
     (write('nat(s(s(o))) fails: 0 point\n'), L3=0)),
	  
      % Test add
      write('\nTEST add\n'),
      (catch(add(o,o,o), E, (print_message(error, E),fail)) ->
	   (write('add(o,o,o) succeeds: 2 points\n'), L4=2);
       (write('add(o,o,o) fails: 0 point\n'), L4=0)),
      (catch(add(o,s(o),s(o)), E, (print_message(error, E),fail)) ->
	   (write('add(o,s(o),s(o)) succeeds: 2 points\n'), L5=2);
       (write('add(o,s(o),s(o)) fails: 0 point\n'), L5=0)),
      (catch((add(s(o),s(o),S), S=s(s(o))), E, (print_message(error, E),fail)) ->
	   (write('add(s(o),s(o),S) succeeds with S=s(s(o)): 2 points\n'), L6=2);
       (write('add(s(o),s(o),S) fails: 0 point\n'), L6=0)),
      
      Scores = [L1,L2,L3,L4,L5,L6],
      sum(Scores, Score),
      write('\nSCORE = '),write(Score),write('\n'));
     write('No name defined\n')).
