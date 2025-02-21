module PE3 where

data Expression = Variable String 
    | Constant Float
    | Sine Expression
    | Cosine Expression
    | Negation Expression
    | Addition Expression Expression
    | Multiplication Expression Expression

class Differential a where
    diff :: a -> a -> a

instance Show Expression where
    show (Variable s) = "Variable " ++ "'" ++ s ++ "'"
    show (Constant f) = "Constant " ++ show f 
    show (Negation e) = "-" ++ (show e)
    show (Sine e) = "sin " ++ (show e)
    show (Cosine e) = "cos " ++ (show e)
    show (Addition e1 e2) = "(" ++ (show e1) ++ " + " ++ (show e2) ++ ")"
    show (Multiplication e1 e2) = "(" ++ (show e1) ++ " * " ++ (show e2) ++ ")"
    show _ = ""

instance Eq Expression where
    (==) (Constant a) (Constant b) = (a == b)
    (==) (Negation e1) (Negation e2) = (e1 == e2)
    (==) (Sine e1) (Sine e2) = (e1 == e2)
    (==) (Cosine e1) (Cosine e2) = (e1 == e2)
    (==) (Variable v1) (Variable v2) = (v1 == v2)
    (==) (Addition e11 e12) (Addition e21 e22) = (e11 == e21 && e12 == e22)
    (==) (Multiplication e11 e12) (Multiplication e21 e22) = (e11 == e21 && e12 == e22)
    (==) _ _ = False

instance Num Expression where
    fromInteger i = Constant (fromIntegral i)
    (*) e1 e2 = Multiplication e1 e2
    (+) e1 e2 = Addition e1 e2
    negate (Constant f) = Constant (-f)
    negate e = Negation e


instance Differential Expression where
    diff (Negation e1) (Variable x) = Negation (diff e1 (Variable x))
    diff (Constant f) (Variable x) = Constant 0
    diff (Addition e1 e2) (Variable x) = Addition (diff e1 (Variable x)) (diff e2 (Variable x))
    diff (Variable s) (Variable x) = if s == x then (Constant 1) else (Constant 0)
    diff (Multiplication e1 e2) (Variable x) = Addition (Multiplication (diff e1 (Variable x)) (e2)) (Multiplication (e1) (diff e2 (Variable x)))
    diff (Cosine e1) (Variable x) = Multiplication (Negation (Sine e1)) (diff e1 (Variable x))
    diff (Sine e1) (Variable x) = Multiplication (Cosine e1) (diff e1 (Variable x)) 
    diff _ _ = Constant 0

shuntingyard :: [String] -> [String] -> [String] -> [String]
shuntingyard [] [] q = q
shuntingyard [] (a:b) q = shuntingyard [] b (q ++ [a])
shuntingyard (a:b) [] c = if a == "*" || a == "+" || a == "-" || a == "sin" || a == "cos" || a == "(" then shuntingyard b [a] c else shuntingyard b [] (c ++ [a])
shuntingyard (a:b) (e:c) d = if a == "(" then shuntingyard b (a:e:c) d else if a == ")" then (if e == "(" then shuntingyard b c d else shuntingyard (a:b) c (d ++ [e])) else if a == "-" then (if e == "-" then shuntingyard (a:b) c (d ++ [e]) else shuntingyard b (a:e:c) d) else if a == "sin" || a == "cos" then (if e == "-" || e == "sin" || e == "cos" then shuntingyard (a:b) c (d ++ [e]) else shuntingyard b (a:e:c) d) else if a == "*" then (if e == "-" || e == "sin" || e == "cos" || e == "*" then shuntingyard (a:b) c (d ++ [e]) else shuntingyard b (a:e:c) d) else if a == "+" then (if e == "(" then shuntingyard b (a:e:c) d else shuntingyard (a:b) c (d ++ [e])) else shuntingyard b (e:c) (d ++ [a])


