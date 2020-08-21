main = do
    x <- getLine
    y <- getLine
    print ((read x :: Integer) * (read y :: Integer))