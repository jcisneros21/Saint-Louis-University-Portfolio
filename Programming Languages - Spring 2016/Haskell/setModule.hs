module Sets
(  Set
,  list2set
,  set2list
,  singS
,  emptyS
,  addToS
,  intersectS
,  diffS
,  subseteq
) where

import qualified Data.List as L

data Set a = Set [a]
             deriving (Show, Eq, Ord)

list2set :: Ord a => [a] -> Set a
list2set = Set . L.nub . L.sort

set2list :: Set a -> [a]
set2list (Set xs) = xs

singS :: a -> Set a
singS x = (Set [x])

emptyS :: Set a
emptyS = (Set [])

addToS a (Set xs) =
    let smallerList = [x|x <- xs, x <= a]
        biggerList = [x|x <- xs, x > a]
    in (Set (smallerList ++ [a] ++ biggerList))

intersectS :: (Ord a) => Set a -> Set a -> Set a
intersectS (Set []) (Set _) = (Set [])
intersectS (Set _) (Set []) = (Set [])
intersectS (Set xs) (Set ys) = (Set [x| x<-xs, y<-ys, x == y])

diffS :: (Ord a) => Set a -> Set a -> Set a
diffS (Set []) (Set _) = (Set [])
diffS (Set _) (Set []) = (Set [])
diffS (Set xs) (Set ys) = (Set [x| x<-xs, elem x ys == False])

subseteq :: (Ord a) => Set a -> Set a -> Bool
subseteq (Set _) (Set []) = False
subseteq (Set []) (Set _) = True
subseteq (Set (x:xs)) (Set ys)
    | elem x ys == True && xs == [] = True
    | elem x ys == True = subseteq (Set xs) (Set ys)
    | elem x ys == False = Fale
