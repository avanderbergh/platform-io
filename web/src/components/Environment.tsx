import { useEffect, useState } from "react"

type Environment = {
    tmp: number
    hum: number
}

export const Environment = () => {

    const [environment, setEnvironment] = useState<Environment>();

    useEffect(() => {
        fetch("/env").then((res) => res.json()).then((data) => {
            setEnvironment(data);
        })
    }, []);

    return (
        <div>
            <h1>Temperature: {environment?.tmp}&deg;C</h1>
            <h1>Humidity: {environment?.hum}%</h1>
        </div>
    )
}