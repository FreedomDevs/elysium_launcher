import React, { createContext, useContext, useEffect, useState } from "react";
import { invoke } from "../utils/invoke.ts";

type InvokeContextType = {
  isReady: boolean;
  hasError: boolean;
};

const InvokeContext = createContext<InvokeContextType>({
  isReady: false,
  hasError: false,
});

export const useInvokeStatus = () => useContext(InvokeContext);

export const InvokeProvider: React.FC<{ children: React.ReactNode }> = ({ children }) => {
  const [isReady, setIsReady] = useState(false);
  const [hasError, setHasError] = useState(false);

  useEffect(() => {
    const check = async () => {
      try {
        await invoke("pingCpp"); // замените на команду, которая точно должна работать
        setIsReady(true);
      } catch (err) {
        console.error("Ошибка при проверке invoke:", err);
        setHasError(true);
      }
    };

    check();
  }, []);

  return (
    <InvokeContext.Provider value={{ isReady, hasError }}>
      {hasError ? (
        <div style={{ padding: "2rem", textAlign: "center", color: "red" }}>
          <h1>Ошибка соединения</h1>
          <p>Нет связи в лаунчером</p>
        </div>
      ) : isReady ? (
        children
      ) : (
        <div style={{ padding: "2rem", textAlign: "center" }}>
          <h2>Загрузка...</h2>
        </div>
      )}
    </InvokeContext.Provider>
  );
};
