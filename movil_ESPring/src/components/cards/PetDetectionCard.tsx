import { View, Text, StyleSheet } from "react-native";

interface Props {
  detected: boolean;
}

export default function PetDetectionCard({ detected }: Props) {
  return (
    <View style={styles.card}>
      <Text style={styles.title}>Detección de Mascota</Text>
      <Text style={styles.value}>
        {detected ? "DETECTADA 🐶" : "SIN DETECCIÓN"}
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
    fontSize: 20,
    marginTop: 8,
  },
});