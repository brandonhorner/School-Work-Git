(* Fall 2019 - Mechanized Reasoning about Programs *)
(* Assignment 08                                   *)

Require Import String.
Definition student := "HW08 Keys"%string.

(** * Proof by simplification *)

Lemma orb_false_left_neutral:
  forall b, orb false b = b.
Proof.
  intro b.
  simpl.
  reflexivity.
Qed.

(** * Proof by rewriting *)

Lemma orb_false_exercise:
  forall b1 b2,
    b1 = false ->
    orb b1 b2 = b2.
Proof.
  intros b1 b2 H.
  rewrite H.
  simpl.
  reflexivity.
Qed.

(** * Proof by case analysis *)

Lemma orb_false_right_neutral:
  forall b, orb b false = b.
Proof.
  intro b.
  destruct b eqn:Hb.
  - reflexivity.
  - reflexivity.
Qed.
