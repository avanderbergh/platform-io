import type { ReactNode } from "react"

export const Button = ({ children, onClick }: { children: ReactNode, onClick: () => void; }) => {
    return <button onClick={onClick}>{children}</button>
}