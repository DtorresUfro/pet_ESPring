import { Tabs } from "expo-router";

export default function Layout() {
  return (
    <Tabs>
      <Tabs.Screen
        name="index"
        options={{
          title: "Básicos",
        }}
      />

      <Tabs.Screen
        name="composite"
        options={{
          title: "Compuestos",
        }}
      />
    </Tabs>
  );
}