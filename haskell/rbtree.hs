data Color = Red | Black
data Tree a = Empty | Node a Color (Tree a) (Tree a)

find :: (Ord a) => Tree a -> a -> Bool
find Empty _ = False
find (Node v1 _ l r) v2 | v1 == v2  = True
                        | v1 > v2   = find l v2
                        | otherwise = find r v2

insert :: (Ord a) => a -> Tree a -> Tree a
insert v tree = Node vv Black ll rr where
                                    Node vv _ ll rr = ins v tree
                                    ins v1 Empty = Node v1 Red Empty Empty
                                    ins v1 t@(Node v2 Red l r)   | v1 == v2  = t
                                                                 | v1 > v2   = Node v2 Red l (ins v1 r)
                                                                 | otherwise = Node v2 Red (ins v1 l) r
                                    ins v1 t@(Node v2 Black l r) | v1 == v2  = t
                                                                 | v1 > v2   = balance $ Node v2 Black l (ins v1 r)
                                                                 | otherwise = balance $ Node v2 Black (ins v1 l) r

balance :: (Ord a) => Tree a -> Tree a
balance (Node v Black (Node v1 Red l1 r1) (Node v2 Red l2 r2)) = (Node v Red (Node v1 Black l1 r1) (Node v2 Black l2 r2))
balance (Node v Black (Node v1 Red (Node v2 Red l1 r1) r2) r3) = (Node v1 Black (Node v2 Red l1 r1) (Node v Red r2 r3))
balance (Node v Black (Node v1 Red l1 (Node v2 Red l2 r2)) r3) = (Node v2 Black (Node v1 Red l1 l2) (Node v Red r2 r3))
balance (Node v Black l1 (Node v1 Red l2 (Node v2 Red l3 r3))) = (Node v1 Black (Node v Red l1 l2) (Node v2 Red l3 r3))
balance (Node v Black l1 (Node v1 Red (Node v2 Red l2 r2) r3)) = (Node v2 Black (Node v Red l1 l2) (Node v1 Red r2 r3))
balance t = t

-- main :: IO ()
-- main = putStrLn "Hello, World!"

delete :: (Ord a) => a -> Tree a -> Tree a
delete v tree = Node - - - - where
                             Node - - - - = del v tree
                             del v1 Empty = Empty
                             del v1 t@(Node v2 c l r) | v1 < v2   = del v1 l
                                                      | v1 > v2   = del v1 r
                                                      | otherwise = delnode t




