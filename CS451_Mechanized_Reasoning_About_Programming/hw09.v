(* Spring 2019 - Mechanized Reasoning about Programs *)
(* Assignment 09                                     *)

Require Import String.
Definition student := "Firstname Lastname"%string.

(** * List of Numbers: Definition and Length *)

Inductive natlist : Type :=
  | nil : natlist
  | cons : nat -> natlist -> natlist.

Fixpoint length (ns: natlist) : nat :=
  match ns with
  | nil => 0
  | cons n ns' => 1+length ns'
  end.

(** * Counting the Zeros of a List of Numbers *)

Fixpoint count0 (ns: natlist) : nat :=
  match ns with
  | nil => 0
  | cons 0 ns' => 1 + count0 ns'
  | cons n ns' => count0 ns'
  end.

(** * Removing the Zeros of a List of Numbers *)

(** Define a function remove0: natlist -> natlist that 
    removes all the zeros of a list of numbers. *)

Fixpoint remove0 (l : natlist) :=
  match l with
  | nil => nil
  | cons O l' => remove0 l'
  | cons n l' => cons n (remove0 l')
  end.

(** * Tests *)
Fact test1: remove0 nil = nil.
Proof. simpl. reflexivity. Qed.

Fact test2: remove0 (cons 1 nil) = cons 1 nil.
Proof. simpl. reflexivity. Qed.

Fact test3: remove0 (cons 0 (cons 1 nil)) = cons 1 nil.
Proof. simpl. reflexivity. Qed.

Fact test4: remove0 (cons 0 (cons 2 (cons 0 nil))) = cons 2 nil.
Proof. simpl. reflexivity. Qed.

Let list :=
  cons 0 (cons 1 (cons 0 (cons 2 (cons 0 (cons 3 (cons 0 nil)))))).
                                                         
Fact test5: remove0 list = cons 1 (cons 2 (cons 3 nil)).
Proof. simpl. reflexivity. Qed.

(** * A Lemma *)

Lemma remove0_length:
  forall ns, length ns = count0 ns + length(remove0 ns).
Proof.
  induction ns as [ | n ns' IH ].
  - simpl. reflexivity.
  - simpl.
    destruct n eqn:Hn.
    + rewrite IH. reflexivity.
    + rewrite IH. simpl. auto with arith.
Qed.