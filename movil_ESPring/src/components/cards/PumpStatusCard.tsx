import { View, Text, StyleSheet } from "react-native";

interface Props {
  isOn: boolean;
}

export default function PumpStatusCard({ isOn }: Props) {
  return (
    <View style={styles.card}>
      <Text style={styles.title}>Estado de la Bomba</Text>
      <Text style={[styles.value, { color: isOn ? "green" : "red" }]}>
        {isOn ? "ACTIVA" : "INACTIVA"}
      </Text>
    </View>
  );
}

const styles = StyleSheet.create({
  card: {
    padding: 16,
    backgroundColor: "#fff",
    borderRadius: 12,
    marginBottom: 12,
  },
  title: {
    fontSize: 16,
    fontWeight: "600",
  },
  value: {
    fontSize: 24,
    marginTop: 8,
    fontWeight: "bold",
  },
});