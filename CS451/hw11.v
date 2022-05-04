(* Spring 2019 - Mechanized Reasoning about Programs *)
(* Assignment 11                                     *)

Require Import String List. Import ListNotations. Import Arith.
Definition student := "Brandon Horner"%string.

(** * Lemmas on Filter *)

Lemma In_filter1:
  forall X test (xs:list X) y,
    In y (filter test xs) ->
    test y = true.
Proof.
  intros X test xs y H. induction xs as [ | x xs IH ].
  - simpl in H. inversion H.
  - simpl in H.
    destruct (test x) eqn:Hx.
    (* eqn:Hy poses a hypothesis Hx to remember 
       that test x = true or test x = false *)
    + simpl in H.  
      destruct H. rewrite <- H.
      apply Hx. apply IH. apply H.
    + destruct IH. apply H. reflexivity.
Qed.

Lemma In_filter2:
  forall X test (xs:list X) y,
    In y (filter test xs) ->
    In y xs.
Proof.
  intros X test xs y H. induction xs as [ | x xs IH ].
  - simpl in H. inversion H.
  - simpl in H.
    destruct (test x) eqn:Hx.
    simpl. simpl in H. 
    intuition.
    intuition.
Qed.