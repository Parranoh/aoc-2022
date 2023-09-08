import Data.List (sort)
import Data.Maybe (catMaybes)

targetY :: Int
targetY = 2000000

maxCoord :: Int
maxCoord = 4000000

type Pos = (Int,Int)

parseLine :: String -> (Pos,Pos)
parseLine l =
    let (sx',l1) = nextInt l
        (sy',l2) = nextInt l1
        (bx',l3) = nextInt l2
        (by',_ ) = nextInt l3
        [sx,sy,bx,by] = map read [sx',sy',bx',by']
    in ((sx,sy),(bx,by))
    where
        digit = flip elem $ '-' : ['0'..'9']
        nextInt = span digit . dropWhile (not . digit)

dist :: Pos -> Pos -> Int
dist (x,y) (x',y') = abs (x - x') + abs (y - y')

-- `I a b` is [a,b)
data Interval = I Int Int deriving (Eq,Ord)
upper :: Interval -> Int
upper (I _ u) = u

len :: Interval -> Int
len (I a b) = b - a

mergeIntervals :: [Interval] -> [Interval]
mergeIntervals = foldr f [] . sort
    where
        f i [] = [i]
        f i@(I a b) xs@(I a' b' : xs')
            | a' <= b   = f (I a (max b b')) xs'
            | otherwise = i : xs

getInterval :: Int -> Pos -> Pos -> Maybe Interval
getInterval y s@(sx,sy) b
    | lb < ub   = Just $ I lb ub
    | otherwise = Nothing
    where
        d  = dist s b
        dy = abs $ sy - y
        lb = sx - d + dy
        ub = sx + d - dy + 1

nub :: Ord a => [a] -> [a]
nub = f . sort
    where
        f [] = []
        f (x:xs) = x : dropWhile (== x) (f xs)

intervals :: Int -> [(Pos,Pos)] -> [Interval]
intervals y = mergeIntervals . catMaybes . map (uncurry $ getInterval y)

between :: Ord a => a -> a -> a -> Bool
between l u x = l <= x && x <= u

solveA,solveB :: [(Pos,Pos)] -> Int
solveA sbs =
    let known = sum . map len . intervals targetY $ sbs
        beacons = length . nub . filter ((==) targetY . snd) . map snd $ sbs
    in known - beacons
solveB sbs
    = (\([x],y) -> 4000000 * x + y)
    . head
    . filter (not . null . fst)
    . map (\y -> (filter (between 0 maxCoord) . map upper . intervals y $ sbs,y))
    $ [0..maxCoord]

main :: IO ()
main = do
    input <- map parseLine . lines <$> getContents
    print $ solveA input
    print $ solveB input
