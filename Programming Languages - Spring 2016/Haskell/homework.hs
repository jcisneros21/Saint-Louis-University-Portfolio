import Data.List.Split (splitOn)
import Data.Char

testLength :: [String] -> String
testLength [] = []
testLength (x:xs)
    | null(xs) = show(length(x)) ++ [head(x)]
    | otherwise = show(length(x)) ++ [head(x)] ++ " " ++ testLength xs

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
makeString (x:xs) 
    | tail(x) == [] = replicate a ' ' ++ makeString xs
    | otherwise = replicate a b ++ makeString xs
      where a = digitToInt(head x)
            b = last x

splitList' :: String -> [String]
splitList' [] = []
splitList' xs = splitOn " " xs

rleInverse :: String -> String
rleInverse [] = []
rleInverse xs = makeString(splitList' xs)

