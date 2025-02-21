module PE2 where

data Tree k v = EmptyTree | Node k v [Tree k v] deriving (Show, Eq)
returnKey (Node k _ _) = k
returnVal (Node _ v _) = v
returnList (Node _ _ []) = []
returnList (Node _ _ t) = t
-- Question 1
selectiveMap :: (a -> Bool) -> (a -> a) -> [a] -> [a]
selectiveMap _ _ [] = []
selectiveMap x y (a:b) = if (x a) then (y a):selectiveMap x y b else a:selectiveMap x y b

-- Question 2
tSelectiveMap :: (k -> Bool) -> (v -> v) -> Tree k v -> Tree k v
tSelectiveMap _ _ EmptyTree = EmptyTree
tSelectiveMap x y t = if (x (returnKey t)) then (Node (returnKey t) (y (returnVal t)) (selectiveHelper x y (returnList t))) else (Node (returnKey t) (returnVal t) (selectiveHelper x y (returnList t)))
selectiveHelper _ _ [] = []
selectiveHelper x y (a:b) = if (x (returnKey a)) then ((Node (returnKey a) (y (returnVal a)) (selectiveHelper x y (returnList a))):(selectiveHelper x y b)) else ((Node (returnKey a) (returnVal a) (selectiveHelper x y (returnList a))):(selectiveHelper x y b))

-- Question 3
tSelectiveMappingFold :: (k -> Bool) -> (k -> v -> r) -> (r -> r -> r) -> r -> Tree k v -> r
tSelectiveMappingFold _ _ _ idt EmptyTree = idt
tSelectiveMappingFold x y z idt t = if (x (returnKey t)) then (foldHelper x y z (z idt (y (returnKey t) (returnVal t))) (returnList t)) else (foldHelper x y z idt (returnList t))
foldHelper _ _  _ idt [] = idt
foldHelper x y  z idt (a:b) = if (x (returnKey a)) then (foldHelper x y z (foldHelper x y z (z idt (y (returnKey a) (returnVal a))) (returnList a)) b) else (foldHelper x y z (foldHelper x y z idt (returnList a)) b)
-- Question 4
-- This question commented out so that your file compiles even before you complete it
-- It shouldn't effect the grades of other questions when commented out
-- When ready, just remove the lines with {- and -} to uncomment the function
searchTree :: (Eq v, Eq k) => v -> Tree k v -> (k -> v)
searchHelper y f1 f2 x = (if (f1 x) /= y then (f1 x) else (f2 x))
searchHelper2 :: Eq k => k -> v -> v -> k -> v
searchHelper2 k v y = \x -> if x == k then v else y    
searchTree def = tSelectiveMappingFold a b c d
    where a = (\k -> (k==k)) 
          b = (\k v -> searchHelper2 k v def)
          c = searchHelper def
          d = (\x -> def)

