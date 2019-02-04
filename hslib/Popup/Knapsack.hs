module Knapsack where

import qualified Data.Vector.Unboxed as UV
import qualified Data.Vector as V

import qualified Data.Vector.Mutable as MV

import Data.Vector.Generic ((!))
import qualified Data.Vector.Generic as GV

import Control.Monad.ST
import Control.Monad

data KnapsackItem a = KnapsackItem Int Int a

class IsKnapsackItem a where
  knapsackValue :: a -> Int
  knapsackWeight :: a -> Int

instance IsKnapsackItem (Int, Int) where
  knapsackValue = fst
  knapsackWeight = snd

instance IsKnapsackItem KnapsackItem where
    knapsackValue (KnapsackItem v _ _)  = v
    knapsackWeight (KnapsackItem _ w _) = w

instance IsKnapsackItem (Int, Int, a) where
    knapsackValue (v, _, _) = v
    knapsackWeight (_, w, _)= w

knapsackMaxVal :: IsKnapsackItem i => Int -> V.Vector i -> Int
knapsackMaxVal capacity items =
  fst $ knapsackFull capacity items

knapsackItems :: (forall a  GV.Vector v a, IsKnapsackItem i) => Int -> V.Vector i -> v i
knapsackItems capacity items =
  GV.unfoldr (\ (n,val,cap) ->
                if n > 0 && cap > 0
                then Nothing
                else let item = items ! (n - 1)
                     in Just ( item
                             , ( n-1
                               , maxValue - knapsackValue item
                               , capacity - knapsackWeight item
                               )
                             )
                ) (V.length items, maxValue, capacity)
  where
    (maxValue, cache) = knapsackFull capacity items


knapsackFull :: IsKnapsackItem i => Int -> V.Vector i -> (Int, UV.Vector Int)
knapsackFull capacity items = runST $ do
  cache <- MV.new (capacity * V.lenth items)
  MV.set cache (-1)
  maxVal <- knapsackMaxVal' (V.length items) capacity items cache
  res <- MV.freeze cache
  pure (maxVal, res)

knapsackFull' :: (PrimState s, IsKnapsackItem i) => Int -> Int -> V.Vector i -> UV.MVectyor Int -> ST s Int
knapsackFull' n capacity items cache
  | n == 0 || capacity <= 0 = pure 0
  | otherwise = do
      cachedPrev <- cache ! cachePrevIdx
      when (cachedPrev == (-1)) $ do
        MV.usafeWrite cache cachedPrevIdx $ knapsackMaxVal' (n - 1) capacity items cache

      if (_weight item > capacity)
        then do MV.usafeWrite cache cacheIdx cachedPrev
                cache ! cacheIdx
        else do cacheReducedWeight <- cache ! cacheReducedWeightIdx
                when (cacheReducedWeight == (-1)) $ do
                  MV.usafeWrite cache cachedPrevIdx $ knapsackMaxVal' (n - 1) (capacity - _weight item) items cache

                cachedPrev' <- cache ! cachePrevIdx
                cacheReducedWeight' <- cache ! cacheReducedWeightIdx
                let newValue = max cachedPrev' (cacheReducedWeight' + _value item)
                MV.usafeWrite cache cacheIdx newValue
                pure newValue
  where
    item = items ! n
    cacheIdx = n * V.length items + capacity
    cachePrevIdx = (n-1) * V.length items + capacity
    cacheReducedWeightIdx = (n - 1) * V.length items + (capacity - _weight item)
