module PE1 where

-- Question 1
findInList :: (Eq a) => [a] -> [a] -> [Int]
listHelper [] _ x _ = x
listHelper (a:b) (c:d) y n = if a==c then listHelper b d (y++[n]) (n+1) else listHelper (a:b) d y (n+1)
findInList x y = listHelper x y [] 0


-- Question 2
findInMatrix :: (Eq a) => [[a]] -> [[a]] -> [[(Int, Int)]]
matrixCreator x 0 = x
matrixCreator x n = matrixCreator ([]:x) (n-1)
appendHelper (x:y) 0 z = ((x ++ [z]):y)
appendHelper (x:y) n z = (x:(appendHelper y (n-1) z))
diagonalHelper a x y (c,d) = if ((d+1) == (length (a !! 0)) && (c+1) == (length a))
                                then ((x ++ [(a !! c !! d)]),(y ++ [(c,d)]))
                            else if (d == 0)
                                then if (c < (length a)) then diagonalHelper a (x ++ [(a !! c !! d)]) (y ++ [(c,d)]) (0,c+1)
                                else diagonalHelper a x y (0,c+1)
                            else 
                                if ((c < (length a)) && (d < (length (a !! 0)))) then diagonalHelper a (x ++ [(a !! c !! d)]) (y ++ [(c,d)]) (c+1,d-1)
                                else diagonalHelper a x y (c+1,d-1)
matrixHelper ([],[]) _ x = x
matrixHelper (a:b,c:d) (e:f,g:h) x = if a == e then matrixHelper (b,d) (f,h) (appendHelper x (fst c) g)
                                                else matrixHelper (a:b,c:d) (f,h) x
findInMatrix x y = matrixHelper (diagonalHelper x [] [] (0,0)) (diagonalHelper y [] [] (0,0)) (matrixCreator [] (length x))


