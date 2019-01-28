module Main where


import Control.Monad
import Text.ParserCombinators.ReadP
import qualified Data.Char as Char

import qualified Data.Vector.Generic as GV
import qualified Data.Vector.Unboxed as UV
import Popup.Knapsack
import qualified Data.List as List

readInput :: ReadP (Int, [(Int,Int,Int)])
readInput = do
  skipSpaces
  capFloat <- read <$> munch (not . Char.isSpace) :: ReadP Double
  numItems <- read <$> munch (not . Char.isSpace)
  items <- forM [0..numItems -1] $ \ i -> do
    value <- read <$> munch (not . Char.isSpace)
    weight <- read <$> munch (not . Char.isSpace)
    pure (value, weight, i)
  pure (floor capFloat, List.sortOn (\ (_,w,_) -> w) items)

readInputs :: ReadP [(Int, [(Int,Int,Int)])]
readInputs = do
  skipSpaces
  rest <- look
  case rest of
    [] -> pure []
    _ -> (:) <$> readInput <*> readInputs

main = do
  inputString <- getContents
  let [(inputs,_)] = readP_to_S readInputs inputString
  forM inputs $ \ (capacity, items) -> do
    items <- knapsackItems capacity (UV.fromList items)
    let indices = fmap (\ (_,_,i) -> i) items
    putStrLn (show $ GV.length indices)
    forM_ indices $ (putStr . (++" ") . show)
    putStr "\n"
