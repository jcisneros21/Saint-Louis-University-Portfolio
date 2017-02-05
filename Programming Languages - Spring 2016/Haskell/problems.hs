import Data.List.Split (splitOn)
import Data.Char

last' :: [a] -> a
last' [] = error "No end for empty lists!"
last' [x] = x
last' (_:xs) = last' xs

secondLast :: [a] -> a
secondLast [] = error "No second to last element for empty lists!"
secondLast [x] = x
secondLast [x,y] = x
secondLast (x:xs) = secondLast xs

elementAt :: [a] -> Int -> a
elementAt [] x = error "There is no list!"
elementAt xs x = xs !! x

addHead :: Int -> [Int] -> [Int]
addHead a [] = []
addHead a (x:xs) = a+x:addHead a xs 

addFirst :: [Int] -> [Int]
addFirst [] = []
addFirst xs = addHead (head xs) xs

addFirst' :: [Int] -> [Int]
addFirst' [] = []
addFirst' all@(x:xs) = head all + x:addFirst' xs

take' :: (Num a, Ord a) => a -> [b] -> [b]
take' n [] = []
take' n (x:xs) 
     | n <= 0  = []
     | otherwise =  x : take' (n-1) xs

reverse' :: [a] -> [a]
reverse' [] = []
reverse' (x:xs) = reverse' xs ++ [x]

stringLen :: [Char] -> [Char]
stringLen "" = ""
stringLen xs = show(length(xs)) ++ [head xs]

testLength :: [String] -> String
testLength [] = []
testLength (x:xs) 
    | null(xs) = show(length(x)) ++ [head(x)]
    | otherwise = show(length(x)) ++ [head(x)] ++ " " ++ testLength xs

-- valueList :: String -> [[Char]] 
-- valueList "" = ""
-- valueList (x:xs) 
	-- | x == head(xs) = 
	-- | otherwise 
     

-- If the user enters "aaabb" then ["aaa","bb"]

-- rle :: String -> [String]
-- rle [] = []
-- rle (x:xs)
   -- | null(xs) = [x:[]]
   -- | x /= head(xs) = x   
   -- | x == head(xs) = [x ++ rle(xs)] 

--splitter :: String -> String
--splitter [] = []
--splitter (x:xs)  

-- check head of tail to see if different value
addComma :: String -> String
addComma [] = []
addComma (x:xs) 
        | null(xs) = x:[]
	| x /= head(xs) = x:',':addComma xs
	| x == head(xs) = x:addComma xs

splitList :: String -> [String]
splitList [] = []
splitList xs = splitOn "," (addComma(xs))

rle :: String -> String
rle [] = []
rle xs = testLength(splitList xs) 

makeString :: [String] -> String
makeString [] = []
makeString (x:xs) = replicate a b ++ makeString xs 
    where a = digitToInt(head x)
          b = last x

splitList' :: String -> [String]
splitList' [] = []
splitList' xs = splitOn " " xs

rleInverse :: String -> String
rleInverse [] = []
rleInverse xs = makeString(splitList' xs) 
