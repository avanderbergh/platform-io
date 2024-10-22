import { useState } from "react";
import { Button } from "./Button";

export const Counter = () => {
    const [count, setCount] = useState(0);

    return (
        <div>
            <h1>{count}</h1>
            <Button onClick={() => setCount(count + 1)}>Increment</Button>
        </div>
    );
}